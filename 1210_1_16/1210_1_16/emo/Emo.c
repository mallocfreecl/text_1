/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:Emo.c
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
*     1.0       2021-07-31      zyg            1.CURRENT_LIMIT_ESTIMAT ：Increases the current limiting threshold for different voltages 
*     1.0       2021-08-05      zyg            1.增加电压突变判断
*     1.0       2021-08-11      zyg            1.增加母线电流滤波偏差判断
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "Emo_RAM.h"
#include "foc_defines.h"
#include "scu_defines.h"
#include "gpt12e_defines.h"
#include "MotorAPI.h"

#include "Common.h"
#include "ADCSample.h"
#include "SDK.h"
#include "DTC.h"
#include "Testmode.h"
/* Defines*/
/******************************************************************/
#define EMO_ISHUNT_ZERO_WAIT_CNT_TH (400u)
#define EMO_ISHUNT_ZERO_MIN (1500u)
#define EMO_ISHUNT_ZERO_MAX (1800u)
#define EMO_FCT_ISHUNT_ZERO_MIN (1575u)  /*FCT零点学习阈值 23.05.12 by zjj*/
#define EMO_FCT_ISHUNT_ZERO_MAX (1725u)
/* Temperature uint:1 degrees centigrade */
#define EMO_I_SHUNT_ZERO_CALIBRATE_AMBT_MIN (-40)
#define EMO_I_SHUNT_ZERO_CALIBRATE_AMBT_MAX (150)
/* Voltage unit:0.1V */
#define EMO_I_SHUNT_ZERO_CALIBRATE_BAT_MIN (70u)
#define EMO_I_SHUNT_ZERO_CALIBRATE_BAT_MAX (190u)

#define DTC_DEFAULT_I_SHUNT_ZERO (1660u)

/*增加零点学习采样次数和时间阈值 23.05.12 by zjj*/
#define DTC_CSA_SAMPLE_CNTS (100u)
#define DTC_CSA_SAMPLE_VALID_CNTS (8u)
#define DTC_CSA_SAMPLE_REDOTIME_THR (10u)
#define I_SHUNT_COEF (1300)	/*A per AD, 1/4096*Vref/OpGain/RShunt 
														  = 1.0/4096*5/60/0.001 
														  = 0.0203451
                             0.0203451 * 1.95 * 32768 = 1300 */
#define I_SHUNT_LIMIT_MIN (144)
#define I_SHUNT_LIMIT_MAX (146)
#define CURRENT_LIMIT_STEP_TIME (50u)
#define CURRENT_LIMIT_STEP_TIME_OVER (2u)
#define CURRENT_LIMIT_POS_INC_STEP (20)
#define CURRENT_LIMIT_POS_DEC_STEP (1)/*1*/
#define CURRENT_LIMIT_NEG_INC_STEP (10)
#define CURRENT_LIMIT_NEG_DEC_STEP (20)/*20*/
#define CURRENT_LIMIT_DIFF (20)/*unit:0.1A*/

#define EMO_CURRENT_LIMIT_ERROR_MAX (40)/*unit:0.1A*/
/*PI输出限制*/
#define EMO_CURRENT_LIMIT_OUT_MAX (8000)
#define EMO_CURRENT_LIMIT_OUT_MIN (1000)

#define	CURRENT_LIMIT_ESTIMAT 0u /*Increases the current limiting threshold for different voltages ,2021-07-31-by zyg  */


/* Variables */
/******************************************************************/
TEmo_Status Emo_Status;
static uint16 Emo_IHighTime = 0, Emo_ILowTime = 0;
static sint16 Emo_DiffIshunt = 0;/*滤波前后电流偏差,-2021-08-08-by zyg*/
static sint16 Emo_OverIshunt = 0;
/*记录上次实际电流*/
static s16 ActIShunt_last = 0;
static uint16 Emo_CSA_Offset = 0;


/*Increases the current limiting threshold for different voltages 2021-07-31-by zyg  */
#if (CURRENT_LIMIT_ESTIMAT) == 1u
#define RC_TABLE_LEN (21) 
static const s16 EMO_RefCurrentTableX[RC_TABLE_LEN] = /* voltages*0.1V 2021-07-30-by zyg  */
{
  80, 85, 90, 95, 100, 105, 110, 115, 120, 125,
  130, 135, 140, 145, 150,155,160,165,170,175,
  180
};
static const s16 EMO_RefCurrentTableY[RC_TABLE_LEN] =		/* Reference current for different voltages 2021-07-30-by zyg  */
{
  -2900, -3100, -3200, -3300, -3350, -3400, -3500, -3550, -3650, -3700,
	-3800, -3903,  -4000,-4100, -4428, -4628, -4958, -5063, -5154, -5253,
  -5363
};
#endif

/* Function declaration */
/******************************************************************/
void Emo_lInitFocPar(void);
void Emo_lInitFocVar(void);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_CaculateIShunt
* Description:
*			计算母线电流
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
void Emo_CaculateIShunt(void)
{
	sint32 I;
	IShunt_t *pIShunt = &Emo_Ctrl.IShuntBuf[Emo_Ctrl.IShuntBufIdx];
	
	static TMat_Lp_Simple IShuntLp ={.CoefA = 100,.CoefB = 100};
	I = (pIShunt->I1*pIShunt->T1 + pIShunt->I2*pIShunt->T2)/((sint32)(CCU6_T12PR));	
	
	Emo_Ctrl.ActIShunt = (I*I_SHUNT_COEF*10)>>15u;/*获取实际电流值,2021-08-11-by zyg*/
	
	I = Mat_ExeLp_without_min_max(&IShuntLp, I);
	
	Emo_Ctrl.IShunt = (I*I_SHUNT_COEF*10)>>15u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_IShuntLimit
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
void Emo_IShuntLimit(void)
{
	/*上次电流与本次电流的差值*/
	s16 ShockIShuntError = 0;
	
  s16 cur_error=0;
	Emo_DiffIshunt  = ABS(Emo_Ctrl.ActIShunt - Emo_Ctrl.IShunt);
	/*当滤波前后电流偏差超过2A,2021-08-12-by zyg*/
	if(Emo_DiffIshunt > CURRENT_LIMIT_DIFF)
	{
		Emo_OverIshunt = Emo_Ctrl.ActIShunt;/*实际电流,2021-08-11-by zyg*/
	}
	else
	{
		Emo_OverIshunt = Emo_Ctrl.IShunt;/*滤波电流,2021-08-11-by zyg*/
	}
	if(Emo_OverIshunt > (I_SHUNT_LIMIT_MAX))
	{
		if(Emo_IHighTime < U16_MAX)
		{
			Emo_IHighTime++;
		}
		
		cur_error = Emo_OverIshunt - I_SHUNT_LIMIT_MAX;/*电流差值 ,21-08-08-by zyg*/
		if(Emo_IHighTime > CURRENT_LIMIT_STEP_TIME)
		{
			Emo_IHighTime = 0;
			
			if(Emo_Ctrl.RefCurr > 0)
			{
				Emo_Ctrl.MaxRefCurrent = Emo_Ctrl.RefCurr - cur_error ;
				if(Emo_Ctrl.MaxRefCurrent < EMO_CURRENT_LIMIT_OUT_MIN)
				{
					Emo_Ctrl.MaxRefCurrent = EMO_CURRENT_LIMIT_OUT_MIN;
				}
			}
			else
			{
				Emo_Ctrl.MinRefCurrent = Emo_Ctrl.RefCurr + cur_error ;
				if(Emo_Ctrl.MinRefCurrent > -EMO_CURRENT_LIMIT_OUT_MIN)
				{
					Emo_Ctrl.MinRefCurrent = -EMO_CURRENT_LIMIT_OUT_MIN;
				}

			}

		}		

		Emo_ILowTime = 0;
		
	}
	else if(Emo_Ctrl.IShunt < I_SHUNT_LIMIT_MIN)
	{
		if(Emo_IHighTime > 0)
		{
			Emo_IHighTime--;
		}
		
		if(   ((Emo_Ctrl.RefSpeed > 0) && (Emo_Ctrl.ActSpeed + 50 < Emo_Ctrl.RefSpeed))
			 || ((Emo_Ctrl.RefSpeed < 0) && (Emo_Ctrl.ActSpeed > Emo_Ctrl.RefSpeed + 50))) 
		{
			if(Emo_ILowTime < U16_MAX)
				Emo_ILowTime++;
		}
		else
		{
			Emo_ILowTime = 0;
		}
		

		if(Emo_ILowTime > CURRENT_LIMIT_STEP_TIME)
		{
			Emo_ILowTime = 0;
						
			if(Emo_Ctrl.RefCurr > 0)
			{
				Emo_Ctrl.MaxRefCurrent += CURRENT_LIMIT_POS_INC_STEP;
	
				
				if(Emo_Ctrl.MaxRefCurrent > EMO_CURRENT_LIMIT_OUT_MAX)
				{
					Emo_Ctrl.MaxRefCurrent = EMO_CURRENT_LIMIT_OUT_MAX;
				}
			}
			else
			{
				Emo_Ctrl.MinRefCurrent -= CURRENT_LIMIT_NEG_INC_STEP;
				
				if(Emo_Ctrl.MinRefCurrent < -EMO_CURRENT_LIMIT_OUT_MAX)
				{
					Emo_Ctrl.MinRefCurrent = -EMO_CURRENT_LIMIT_OUT_MAX;
				}
			}
		}
	}
	else
	{
	
	}		
	
	{
		/*当电流急剧抖动时，减少输出*/
		ShockIShuntError = ABS(ActIShunt_last - Emo_Ctrl.ActIShunt);

		if(ShockIShuntError > EMO_CURRENT_LIMIT_ERROR_MAX)
		{
			if(Emo_Ctrl.RefCurr > 0)
				{
					Emo_Ctrl.MaxRefCurrent = Emo_Ctrl.RefCurr - CURRENT_LIMIT_POS_DEC_STEP;
					if(Emo_Ctrl.MaxRefCurrent < EMO_CURRENT_LIMIT_OUT_MIN)
					{
						Emo_Ctrl.MaxRefCurrent = EMO_CURRENT_LIMIT_OUT_MIN;
					}
				}
				else
				{
					Emo_Ctrl.MinRefCurrent = Emo_Ctrl.RefCurr + CURRENT_LIMIT_POS_DEC_STEP;
					if(Emo_Ctrl.MinRefCurrent > -EMO_CURRENT_LIMIT_OUT_MIN)
					{
						Emo_Ctrl.MinRefCurrent = -EMO_CURRENT_LIMIT_OUT_MIN;
					}
				}
		}
	}
	ActIShunt_last = Emo_Ctrl.ActIShunt;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_Init
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
void Emo_Init(void)
{
  GPT12E_T2_Start();
  /* Initialize FOC parameters */
	ADCSample_PeriodTask();
  Emo_lInitFocPar();
  /* Initialize motor state */
  Emo_Status.MotorState = EMO_MOTOR_STATE_STOP;
} /* End of Emo_Init */

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_StartMotor
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
void Emo_StartMotor(void)
{
  if(Emo_Status.MotorState != (EMO_MOTOR_STATE_STOP))
  {
    return ;
  }
  else
  {

  }

  /* Initialize variables */
  Emo_lInitFocVar();

  CCU6_LoadShadowRegister_CC60((CCU6_T12PR)/2u);
  CCU6_LoadShadowRegister_CC61((CCU6_T12PR)/2u);
  CCU6_LoadShadowRegister_CC62((CCU6_T12PR)/2u);
  CCU6_EnableST_T12();

  /* Set Timer13 Trigger calculated previous period zero-match*/
  CCU6_SetT13Trigger(0x7au);
  /* Set T13 Compare Value */
  CCU6_SetT13Compare((CCU6_T12PR));

  TIMER21->T2CON.bit.TR2 = 0u;
  uint16 Time = 0xFFFFu - ((CCU6_T12PR)/2u);
  TIMER21->T2H.reg = (uint8) (Time>>8u);
  TIMER21->T2L.reg = (uint8) (Time&0xFFu);
  TIMER21->T2CON.bit.TR2 = 1u;

  /* Start PWM */
  CCU6_StartTmr_T12();
  /* Disable output pattern */
  CCU6_Multi_Ch_PWM_Shadow_Reg_Load(0x0000u);
  /* Enable Shadow Transfer Request for MCMPS */
  CCU6_MCM_PWM_Str_SW_En();
  /* Clear all Error and Warning Flags */
  BDRV_Clr_Sts(0x1F1FFC00u);

  /* Enable bridge (after starting PWM according to UM) */
  BDRV_Set_Bridge(Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM);

  /* Set start state */
  Emo_Status.MotorState = EMO_MOTOR_STATE_START;
} 

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_StopMotor
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
void Emo_StopMotor(void)
{
  if((Emo_Status.MotorState != (EMO_MOTOR_STATE_RUN))
  && (Emo_Status.MotorState != (EMO_MOTOR_STATE_START))
  && (Emo_Status.MotorState != (EMO_MOTOR_STATE_FAULT))
    )
  {
    /* Error detected: return with error */
    return ;
  }
  else
  {

  }

  /* Disable bridge */
  BDRV_Set_Bridge(Ch_Off, Ch_Off, Ch_Off, Ch_Off, Ch_Off, Ch_Off);
  /* Stop PWM */
  CCU6_StopTmr_T12();
  /* Set stop state */
  Emo_Status.MotorState = EMO_MOTOR_STATE_STOP;

  Emo_Ctrl.ActSpeed = 0;
  Emo_Ctrl.IShunt = 0;
	Emo_Ctrl.ActIShunt = 0;
	
	ActIShunt_last = 0;
} /* End of Emo_StopMotor */


/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FctEmo_calIShuntZero
* Description:
* 1.采集母线电流采样运放零点值
*
* Inputs:
*
*
* Outputs:
*
* Limitations:
* 1.该函数应在FCT测试模式调用
********************************************************************
END_FUNCTION_HDR*/
void FctEmo_calIShuntZero(void)
{
	u8 ret = 0u;
  u16 WaitCnt = 0u;
  u8 ADValid = 0u;
  u8 Redo_Time = 0u;
  u8 Emo_CSA_SampleCnt = 0u;
  u8 Emo_CSA_SampleValidCnt = 0u;
  u32 Emo_CSA_Sum = 0u;

	ADC1_Software_Mode_Sel();

	while ((ADC1_Busy() == true) && (ret == 0u))
	{
		WaitCnt++;
		if(WaitCnt > EMO_ISHUNT_ZERO_WAIT_CNT_TH)
		{
			ret = 1u;
			CSAZeroFault.bit.CSAZeroAdcBusyError = 1u; /*零点学习超时置位 23.05.12 by zjj*/
		}
	}
	
	WaitCnt = 0u;
	/*多次采样零点值 23.05.12 by zjj*/
	while((Emo_CSA_SampleCnt < DTC_CSA_SAMPLE_CNTS) && (ret == 0u))
	{
		Redo_Time = 0;
		do
		{
			/* set Ch1 (CSA) for software conversion mode */
			ADC1_SW_Ch_Sel(ADC1_CSA);
			/* start conversion */
			ADC1_SOC_Set();

			/* wait until conversion is done */
			while ((ADC1_isEndOfConversion() == false) && (ret == 0u))
			{
				WaitCnt++;
				if(WaitCnt > EMO_ISHUNT_ZERO_WAIT_CNT_TH)
				{
					ret = 1u;
					CSAZeroFault.bit.CSAZeroAdcEocError = 1u;
				}
			}
			WaitCnt = 0u;
			
			if(ADC1->RES_OUT1.bit.VF1 == 0u) /*结果寄存器位字段OUT_CH1是否存在有效数据 23.05.12 by zjj*/
			{
				Redo_Time++;
				ADValid = 0u;
			}
			else
			{
				ADValid = 1u;
			}
		}while((ADValid == false) && (Redo_Time < DTC_CSA_SAMPLE_REDOTIME_THR));
			
		if(ADValid != 0u)
		{
			Emo_CSA_Offset = ADC1->RES_OUT1.bit.OUT_CH1;
			Emo_CSA_SampleValidCnt++;
			Emo_CSA_Sum += Emo_CSA_Offset;
		}

		Emo_CSA_SampleCnt ++;
	}
	

	
	ADC1_Sequencer_Mode_Sel();

	/*零点学习有效值取平均赋值 23.05.12 by zjj*/
	if(Emo_CSA_SampleValidCnt >= DTC_CSA_SAMPLE_VALID_CNTS)
	{
		Emo_CSA_Offset = Emo_CSA_Sum / Emo_CSA_SampleValidCnt;
	}
	else
	{
		CSAZeroFault.bit.CSAZeroSampleCntErrorr = 1u;
		Emo_CSA_Offset = 0;
	}

  /*判断FCT零点是否超出阈值 23.05.12 by zjj*/

  ADCResults.ErrFlags.IShuntZero = (Emo_CSA_Offset > EMO_FCT_ISHUNT_ZERO_MAX) || (Emo_CSA_Offset < EMO_FCT_ISHUNT_ZERO_MIN);


	/*零点学习错误标志置位 23.05.12 by zjj*/
	if((ADCResults.ErrFlags.IShuntZero != 0u) || (ret != 0))
	{
		CSAZeroFault.bit.CSAZeroCalibrationFault = 1u;
	}
	else
	{
		CSAZeroFault.bit.CSAZeroCalibrationFault = 0u;
		DTCVar.IShuntZero = Emo_CSA_Offset;	
		Emo_Svm.CsaOffset = Emo_CSA_Offset;
		DtcIShuntWriteEn = 1u;	
	}
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:calcIShuntZero
* Description:
* 1.采集母线电流采样运放零点值
* 2.如果电源电压、环境温度在设置的范围内，则进行母线电流采样运放零点标定
* 3.母线电流采样运放零点无法标定或标定错误时，使用DTC中的存储值
*
* Inputs:
*
*
* Outputs:
* 1.AdcResults.RawAD.IShuntZero：电流零点AD值
* 2.AdcResults.ErrFlags.IShuntZero：电流零点值错误标志，1：错误，0：正常
*
* Limitations:
* 1.该函数应在DTC初始化后调用
********************************************************************
END_FUNCTION_HDR*/
void Emo_calIShuntZero(void)
{
	u8 ret = 0u;
  u16 WaitCnt = 0u;
  u8 ADValid = 0u;
  u8 Redo_Time = 0u;
  u8 Emo_CSA_SampleCnt = 0u;
  u8 Emo_CSA_SampleValidCnt = 0u;
  u32 Emo_CSA_Sum = 0u;
  u8 Err = 0u;

  if((ADCResults.Voltage.Bat > EMO_I_SHUNT_ZERO_CALIBRATE_BAT_MIN) &&
      (ADCResults.Voltage.Bat < EMO_I_SHUNT_ZERO_CALIBRATE_BAT_MAX) &&
      (ADCSample_Temp > EMO_I_SHUNT_ZERO_CALIBRATE_AMBT_MIN) &&
      (ADCSample_Temp < EMO_I_SHUNT_ZERO_CALIBRATE_AMBT_MAX))
  {
		ADC1_Software_Mode_Sel();

		while ((ADC1_Busy() == true) && (ret == 0u))
		{
			WaitCnt++;
			if(WaitCnt > EMO_ISHUNT_ZERO_WAIT_CNT_TH)
			{
				ret = 1u;
				CSAZeroFault.bit.CSAZeroAdcBusyError = 1u; /*零点学习超时置位 23.05.12 by zjj*/
			}
		}
	
		WaitCnt = 0u;
		/*多次采样零点值 23.05.12 by zjj*/
		while((Emo_CSA_SampleCnt < DTC_CSA_SAMPLE_CNTS) && (ret == 0u))
		{
			Redo_Time = 0;
			do
			{
				/* set Ch1 (CSA) for software conversion mode */
				ADC1_SW_Ch_Sel(ADC1_CSA);
				/* start conversion */
				ADC1_SOC_Set();

				/* wait until conversion is done */
				while ((ADC1_isEndOfConversion() == false) && (ret == 0u))
				{
					WaitCnt++;
					if(WaitCnt > EMO_ISHUNT_ZERO_WAIT_CNT_TH)
					{
						ret = 1u;
						CSAZeroFault.bit.CSAZeroAdcEocError = 1u;
					}
				}
				WaitCnt = 0u;
			
				if(ADC1->RES_OUT1.bit.VF1 == 0u) /*结果寄存器位字段OUT_CH1是否存在有效数据 23.05.12 by zjj*/
				{
					Redo_Time++;
					ADValid = 0u;
				}
				else
				{
					ADValid = 1u;
				}
			}while((ADValid == false) && (Redo_Time < DTC_CSA_SAMPLE_REDOTIME_THR));
			
			WaitCnt = 0u;
			
			if(ADValid != 0u)
			{
				Emo_CSA_Offset = ADC1->RES_OUT1.bit.OUT_CH1;
				Emo_CSA_SampleValidCnt++;
				Emo_CSA_Sum += Emo_CSA_Offset;
			}

			Emo_CSA_SampleCnt ++;
		}
	
		ADC1_Sequencer_Mode_Sel();

		/*零点学习有效值取平均赋值 23.05.12 by zjj*/
		if(Emo_CSA_SampleValidCnt >= DTC_CSA_SAMPLE_VALID_CNTS)
		{
			Emo_CSA_Offset = Emo_CSA_Sum / Emo_CSA_SampleValidCnt;
			
			/*零点学习超过上限用上限值，低于下限用下限值 23.05.12 by zjj*/
			if(Emo_CSA_Offset > EMO_ISHUNT_ZERO_MAX)
			{
				Emo_CSA_Offset = EMO_ISHUNT_ZERO_MAX;
				CSAZeroFault.bit.CSAZeroHigh = 1u;
			}
			else if(Emo_CSA_Offset < EMO_ISHUNT_ZERO_MIN)
			{
				Emo_CSA_Offset = EMO_ISHUNT_ZERO_MIN;
				CSAZeroFault.bit.CSAZeroLow = 1u;
			}
			else
			{
		
			}
		}
		else
		{
			CSAZeroFault.bit.CSAZeroSampleCntErrorr = 1u;
			Emo_CSA_Offset = DTC_DEFAULT_I_SHUNT_ZERO;
		}
  }
  else
  {
		CSAZeroFault.bit.CSAZeroVoltOrTempError = 1u;   /*零点学习电压温度条件置位 23.05.12 by zjj*/
    Err = 1u;
  }

  if (Err != 0u)
  {
    Emo_CSA_Offset = DTC_DEFAULT_I_SHUNT_ZERO;
  }
	
	/*判断FCT零点是否超出阈值 23.05.12 by zjj*/
  ADCResults.ErrFlags.IShuntZero = (Emo_CSA_Offset > EMO_ISHUNT_ZERO_MAX) || (Emo_CSA_Offset < EMO_ISHUNT_ZERO_MIN);
	
	/*零点学习错误标志置位 23.05.12 by zjj*/
	if((ADCResults.ErrFlags.IShuntZero != 0u) || (Err != 0u))
	{
		CSAZeroFault.bit.CSAZeroCalibrationFault = 1u;
	}
	else
	{
		CSAZeroFault.bit.CSAZeroCalibrationFault = 0u;
	}
	DTCVar.IShuntZero = Emo_CSA_Offset;	
	Emo_Svm.CsaOffset = Emo_CSA_Offset;
	IsDTCUpdated = 1u;	
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_StopMotor
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
void Emo_lInitFocPar(void)
{
		float KI;
    /* set to 13V */
    float KU = 13.0;
    /* float KI; */
    /* set to 0.1V/s */
    float KPSIE = 0.1;
    float KUZ;
    static float x;
    /* float CoAFlux = 200.0; */
    float OpGain;
	
    Emo_Status.MotorStartError = 0;
    Emo_Svm.MaxAmp = ((CCU6_T12PR) / EMO_CFG_FOC_TABLE_SCALE);//5,095.5430917872086573260115965203
    /*vectorial voltage limitation, for block limitation algorithm*/
    /*0.9091 * MaxAmp*/
    Emo_Svm.MaxAmp9091pr = (29789 * Emo_Svm.MaxAmp) >> MAT_FIX_SHIFT;
    /*0.4164 * MaxAmp*/
    Emo_Svm.MaxAmp4164pr = (13643 * Emo_Svm.MaxAmp) >> MAT_FIX_SHIFT;
    Emo_Svm.MaxAmpQuadrat = (uint32)Emo_Svm.MaxAmp * Emo_Svm.MaxAmp;
    Emo_Svm.Kfact256 = 8388608 / Emo_Svm.MaxAmp; //2 exp23/MaxAmp
    OpGain = 10.0;
    CSA_Set_Gain(0u);
    /* 1.25 => 125mV, secured OpAmp differential input voltage range */
    x = Emo_Focpar_Cfg.NominalCurrent * Emo_Focpar_Cfg.Rshunt * 10.0;

    if (x < 1.25)
    {
        OpGain = 10.0;
        CSA_Set_Gain(0u);
    }
    else
    {

    }

    x = Emo_Focpar_Cfg.NominalCurrent * Emo_Focpar_Cfg.Rshunt * 20.0;

    if (x < 1.25)
    {
        OpGain = 20.0;
        CSA_Set_Gain(1u);
    }
    else
    {
      
    }

    x = Emo_Focpar_Cfg.NominalCurrent * Emo_Focpar_Cfg.Rshunt * 40.0;

    if (x < 1.25)
    {
        OpGain = 40.0;
        CSA_Set_Gain(2u);
    }
    else
    {
      
    }

    x = Emo_Focpar_Cfg.NominalCurrent * Emo_Focpar_Cfg.Rshunt * 60.0;

    if (x < 1.25)
    {
        OpGain = 60.0;
        CSA_Set_Gain(3u);
    }
    else
    {
      
    }

    /* KI current regulator parameter          **
    ** 5.0 => 5V ADC referenc voltage          */
    KI = 5.0 * 2.0 / (Emo_Focpar_Cfg.Rshunt * OpGain);
    /* KUZ Battery Voltage parameter */
    KUZ = 32768.0 * 12.0 / 1612.0; /* 12V = 1612Ink ADW *///243.93052109181141439205955334988
    /* Measuring the CSA Offset */	
    if((DTC_CalIShuntZeroEn != 0u) || (ECUFaults.bit.DFlash != 0u))
		{
		Emo_calIShuntZero();
		}
		Emo_Svm.CsaOffset = DTCVar.IShuntZero;
    /* Initialize parameters for FOC */
    x = 32768.0 * KI * Emo_Focpar_Cfg.PhaseRes / KU;

    if (x > 32767.0)
    {
        x = 32767.0;
    }
    else
    {

    }

    Emo_Foc.PhaseRes = x;
    x = 32768.0 * KI * Emo_Focpar_Cfg.PhaseInd / KPSIE;

    if (x > 32767.0)
    {
        x = 32767.0;
    }
    else
    {
      
    }

    Emo_Foc.PhaseInd = x;
    Emo_Foc.Kdcdivident1 = KU * 1.7320508 * Emo_Svm.MaxAmp / KUZ * 32768 / 2.0;
    Emo_Foc.Kdcfactor2 = KUZ * 32768.0 * 32768.0 / (KU * 1.7320508 * Emo_Svm.MaxAmp * 64);
    Emo_Foc.Kdcfactoriqc = 32768.0 * KUZ / (1.7320508 * KU * 32.0);
    Emo_Foc.PolePair = Emo_Focpar_Cfg.PolePair;
    x = Emo_Focpar_Cfg.StartCurrent / KI * 32768.0;

    if (x > 32767)
    {
        x = 32767;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_STARTCURRENT);
    }
    else
    {
      
    }

    if (x < 1)
    {
        x = 1;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_STARTCURRENT);
    }
    else
    {
      
    }

    Emo_Foc.StartCurrent = x;
    x = Emo_Focpar_Cfg.TimeSpeedzero * (M_Low_Task_Freq);

    if (x > 32767)
    {
        x = 32767;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_STARTTIME);
    }
    else
    {
      
    }

    if (x < 1)
    {
        x = 1;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_STARTTIME);
    }
    else
    {
      
    }

    Emo_Foc.TimeSpeedzero = x;
    Emo_Foc.StartEndSpeed = Emo_Focpar_Cfg.StartSpeedEnd;
    x =  Emo_Focpar_Cfg.StartSpeedSlewRate * 65536.0 / (M_Low_Task_Freq);

    if (x > 2147483647)
    {
        x = 2147483647;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_SPEEDSLEWRATE);
    }
    else
    {
      
    }

    if (x < 1)
    {
        x = 1;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_SPEEDSLEWRATE);
    }
    else
    {
      
    }

    Emo_Foc.StartSpeedSlewRate = x;
    Emo_Foc.SpeedtoFrequency = (32768.0 * (1.0 / (M_High_Task_Freq)) * 32768.0 / 30.0) * FOC_POLE_PAIRS;
		
		Emo_Foc.RealFluxLp.CoefA = 470;
		Emo_Foc.ImagFluxLp.CoefA = 470;	
		Emo_Foc.RealFluxLp.CoefB = 50;
		Emo_Foc.ImagFluxLp.CoefB = 50;
		
    Emo_Ctrl.SpeedPi.Kp = Emo_Focpar_Cfg.SpeedPi_Kp;
    Emo_Ctrl.SpeedPi.Ki = Emo_Focpar_Cfg.SpeedPi_Ki;
    Emo_Ctrl.SpeedPi.PiMin = 32767.0 * Emo_Focpar_Cfg.MinRefStartCurr / KI;
    Emo_Ctrl.SpeedPi.PiMax = 32767.0 * Emo_Focpar_Cfg.MaxRefStartCurr / KI;
    x = 32767.0 * Emo_Focpar_Cfg.MaxRefCurr / KI; 

    if (x > 32767)
    {
        x = 32767;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_REFCURRENT);
    }
    else
    {
      
    }

    Emo_Ctrl.MaxRefCurrent = x;
    x = 32767.0 * Emo_Focpar_Cfg.MinRefCurr / KI;

    if (x < -32767)
    {
        x = -32767;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_REFCURRENT);
    }
    else
    {
      
    }

    Emo_Ctrl.MinRefCurrent = x;
    x = 32767.0 * Emo_Focpar_Cfg.MaxRefStartCurr / KI;

    if (x > 32767)
    {
        x = 32767;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_REFCURRENT);
    }
    else
    {
      
    }

    Emo_Ctrl.MaxRefStartCurrent = x;
    x = 32767.0 * Emo_Focpar_Cfg.MinRefStartCurr / KI;

    if (x < -32767)
    {
        x = -32767;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_REFCURRENT);
    }
    else
    {
      
    }

    Emo_Ctrl.MinRefStartCurrent = x;
    Emo_Ctrl.Speedlevelmaxstart = Emo_Focpar_Cfg.SpeedLevelPos;
    Emo_Ctrl.Speedlevelminstart = Emo_Focpar_Cfg.SpeedLevelNeg;
    Emo_Ctrl.SpeedLevelSwitchOn = Emo_Focpar_Cfg.SpeedLevelSwitchOn;

    if (Emo_Ctrl.Speedlevelmaxstart < Emo_Ctrl.Speedlevelminstart)
    {
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_SPEED_POINTS);
    }
    else
    {
      
    }

    if (Emo_Ctrl.Speedlevelmaxstart < Emo_Ctrl.SpeedLevelSwitchOn)
    {
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_SPEED_POINTS);
    }
    else
    {
      
    }

    if ((Emo_Ctrl.Speedlevelminstart + Emo_Ctrl.SpeedLevelSwitchOn) > 0)
    {
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_SPEED_POINTS);
    }
    else
    {
      
    }

    if (Emo_Foc.StartEndSpeed < Emo_Ctrl.SpeedLevelSwitchOn)
    {
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_SPEED_POINTS);
    }
    else
    {
      
    }

    if (Emo_Ctrl.MaxRefCurrent < Emo_Ctrl.MaxRefStartCurrent)
    {
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_LIMITS_REFCURRENT);
    }
    else
    {
      
    }

    if (Emo_Ctrl.MinRefCurrent > Emo_Ctrl.MinRefStartCurrent)
    {
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_LIMITS_REFCURRENT);
    }
    else
    {
      
    }

    x = Emo_Focpar_Cfg.AdjustmCurrentControl;

    if ((x > 1) || (x < 0.01))
    {
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_VALUE_CU_ADCC);
    }
    else
    {
      
    }
		
		Emo_Ctrl.RealCurrPi.Kp = 300;
		Emo_Ctrl.RealCurrPi.Ki = 500; 
		
    /* id = PI regulator limits */
    Emo_Ctrl.RealCurrPi.IMin = -28272;
    Emo_Ctrl.RealCurrPi.IMax = 28272;
    Emo_Ctrl.RealCurrPi.PiMin = -28272;
    Emo_Ctrl.RealCurrPi.PiMax = 28272;
    Emo_Ctrl.ImagCurrPi.Kp =  Emo_Ctrl.RealCurrPi.Kp;
    Emo_Ctrl.ImagCurrPi.Ki =  Emo_Ctrl.RealCurrPi.Ki;

    /* iq = PI regulator limits, iq needs higher range than id */
    Emo_Ctrl.ImagCurrPi.IMin = -16580;
    Emo_Ctrl.ImagCurrPi.IMax = 16580;
    Emo_Ctrl.ImagCurrPi.PiMin = -16580;
    Emo_Ctrl.ImagCurrPi.PiMax = 16580;
    x = (1.0 / ((M_Low_Task_Freq))) / (Emo_Focpar_Cfg.TimeConstantSpeedFilter) * 32768.0;

    if (x > 32767)
    {
        x = 32767;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_T_SPEED_LP);
    }
    else
    {
      
    }

    if (x < 1)
    {
        x = 1;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | (EMO_ERROR_T_SPEED_LP);
    }
    else
    {

    }

    Emo_Ctrl.RotCurrImagLpdisplay.CoefA=10;
    Emo_Ctrl.RotCurrImagLpdisplay.CoefB=10;

    Emo_Ctrl.SpeedLp.CoefA = x;
    Emo_Ctrl.SpeedLp.CoefB = x;
    Emo_Ctrl.FluxbtrLp.CoefA = 1000;
    Emo_Ctrl.FluxbtrLp.CoefB = 1000;
    Emo_Ctrl.SpeedLpdisplay.CoefA = 1000;
    Emo_Ctrl.SpeedLpdisplay.CoefB = 1000;
    Emo_Ctrl.Pllkp = 50;
    x = 60.0 * (M_High_Task_Freq) / 64.0;

    Emo_Ctrl.Factorspeed = x;
    /* Speed PLL adjustment based on max. mech. speed             **
    ** 60.0 => conversion frequency into rpm (seconds to minutes) **
    ** 4.0 => 1/4 electrical rotation => 90                      */
    x = 60.0 * (M_High_Task_Freq) / (Emo_Focpar_Cfg.MaxSpeed * Emo_Focpar_Cfg.PolePair * 4.0);

    if (x >= 32.0)
    {
        Emo_Ctrl.Exppllhigh = 5u;
        Emo_Ctrl.Anglersptr = 32u;
        Emo_Ctrl.Expspeedhigh = 0u;
    }
    else
    {
        if (x >= 16.0)
        {
            Emo_Ctrl.Exppllhigh = 4u;
            Emo_Ctrl.Anglersptr = 16u;
            Emo_Ctrl.Expspeedhigh = 1u;
        }
        else
        {
            if (x >= 8.0)
            {
                Emo_Ctrl.Exppllhigh = 3u;
                Emo_Ctrl.Anglersptr = 8u;
                Emo_Ctrl.Expspeedhigh = 2u;
            }
            else
            {
                if (x >= 4.0)
                {
                    Emo_Ctrl.Exppllhigh = 2u;
                    Emo_Ctrl.Anglersptr = 4u;
                    Emo_Ctrl.Expspeedhigh = 3u;
                }
                else
                {
                    Emo_Ctrl.Exppllhigh = 1u;
                    Emo_Ctrl.Anglersptr = 2u;
                    Emo_Ctrl.Expspeedhigh = 4u;
                }
            }
        }
    }
} 

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_lInitFocVar
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
void Emo_lInitFocVar(void)
{
    uint16 i;
    /* Initialize variables for SVM */
    Emo_Svm.Angle = 0u;
    Emo_Svm.Amp = 0u;
    Emo_Svm.Sector = 0u;
    Emo_Svm.T1 = 0u;
    Emo_Svm.T2 = 0u;
    Emo_Svm.PhaseCurr.A = 0;
    Emo_Svm.PhaseCurr.B = 0;
		/*每次启动前电机使用之前的零点 20230517 by zjj*/
    Emo_Svm.CounterOffsetAdw=0u;
    Emo_Svm.CsaOffsetAdwSumme=0u;
    Emo_Svm.CsaOffsetAdw = Emo_Svm.CsaOffset;

    CompareVal.EdgeSelect[0].comp60up = (CCU6_T12PR)/2u;
    CompareVal.EdgeSelect[0].comp60down = (CCU6_T12PR)/2u;
    CompareVal.EdgeSelect[0].comp61up = (CCU6_T12PR)/2u;
    CompareVal.EdgeSelect[0].comp61down = (CCU6_T12PR)/2u;
    CompareVal.EdgeSelect[0].comp62up = (CCU6_T12PR)/2u;
    CompareVal.EdgeSelect[0].comp62down = (CCU6_T12PR)/2u;
    CompareVal.EdgeSelect[0].CompT13ValueUp = (CCU6_T12PR);
    CompareVal.EdgeSelect[0].CompT13ValueDown = (CCU6_T12PR);
    CompareVal.EdgeSelect[1] = CompareVal.EdgeSelect[0];

    /* Initialize variables for FOC */
    Emo_Foc.StatCurr.Real = 0;
    Emo_Foc.StatCurr.Imag = 0;
    Emo_Foc.RotCurr.Real = 0;
    Emo_Foc.RotCurr.Imag = 0;
    Emo_Foc.StatVolt.Real = 0;
    Emo_Foc.StatVolt.Imag = 0;
    Emo_Foc.RotVolt.Real = 0;
    Emo_Foc.RotVolt.Imag = 0;
    Emo_Foc.Angle = 0u;
		Emo_Foc.Angle1 = 0u;
		Emo_Foc.Angle2 = 0u;
    Emo_Foc.FluxAngle = 0u;
    Emo_Foc.StoredAngle = 0u;
    Emo_Foc.RealFluxLp.Out = 0;
    Emo_Foc.ImagFluxLp.Out = 0;
    Emo_Foc.StartSpeedSlopeMem = 0;
    Emo_Foc.CountStart = Emo_Foc.TimeSpeedzero;
    /* define 30 degree as start angle */
    Emo_Foc.StartAngle = (uint16)(30.0 / 360.0 * 65536.0);

    for (i = 0u; i < 31u; i++)
    {
        Emo_Ctrl.AngleBuffer[i] = 0u;
    }
    Emo_Ctrl.ActSpeed = 0;
    Emo_Ctrl.RefCurr = Emo_Foc.StartCurrent;
    Emo_Ctrl.DemagCurr = 0;
    Emo_Ctrl.RealCurrPi.IOut = 0;
    Emo_Ctrl.ImagCurrPi.IOut = 0;
    Emo_Ctrl.SpeedLp.Out = 0;
    Emo_Ctrl.SpeedLpdisplay.Out = 0;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_MLowTask
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
void Emo_MLowTask(void)
{
  sint32 jj;
  sint16 Speed;
	
	static sint16 RefSpeedLast = 0;
	
	if(((RefSpeedLast > 0) && (Emo_Ctrl.RefSpeed < 0))
	|| ((RefSpeedLast < 0) && (Emo_Ctrl.RefSpeed > 0))
	)
	{
		Emo_StopMotor();
		Emo_StartMotor();
	}
	else/* add else */
	{

	}
	RefSpeedLast = Emo_Ctrl.RefSpeed;
	
	
  if(Emo_Status.MotorState == (EMO_MOTOR_STATE_START))
  {
		Emo_CaculateIShunt();
		
    /* Open loop: */
    /* Perform ramp to start end speed */
    if (Emo_Foc.CountStart == 0u)
    {
      /* I/F Start-up */
      if (Emo_Ctrl.RefSpeed > 0)
      {
        /* positive rotation */
        /* apply start up ramp */
        Emo_Foc.StartSpeedSlope = Mat_Ramp(Emo_Foc.StartEndSpeed, Emo_Foc.StartSpeedSlewRate, &Emo_Foc.StartSpeedSlopeMem);

        /* calculate frequency out of ramp */
        Emo_Foc.StartFrequencySlope = __ssat(Mat_FixMulScale(Emo_Foc.StartSpeedSlope, Emo_Foc.SpeedtoFrequency, 0u), MAT_FIX_SAT);

        if(Emo_Foc.StartSpeedSlope == Emo_Foc.StartEndSpeed)
        {
          /* end speed reached */
#if (EMO_RUN==1)
          Emo_Status.MotorState = EMO_MOTOR_STATE_RUN;                       /*Start Foc */	
					
#endif
          /* preload speed regulator */
          Emo_Ctrl.SpeedPi.IOut = Emo_Ctrl.RefCurr << 14;
        }
				else/* add else */
				{

				}
      }
      else
      {
        /* negative rotation */
        /* apply start up ramp */
        Emo_Foc.StartSpeedSlope = Mat_Ramp(-Emo_Foc.StartEndSpeed, Emo_Foc.StartSpeedSlewRate, &Emo_Foc.StartSpeedSlopeMem);
				
        /* calculate frequency out of ramp */
        Emo_Foc.StartFrequencySlope = __ssat(Mat_FixMulScale(Emo_Foc.StartSpeedSlope, Emo_Foc.SpeedtoFrequency, 0u), MAT_FIX_SAT);

        if(Emo_Foc.StartSpeedSlope == -Emo_Foc.StartEndSpeed)
        {
#if (EMO_RUN==1)
          Emo_Status.MotorState = EMO_MOTOR_STATE_RUN;                       /*Start Foc */							
#endif
          Emo_Ctrl.SpeedPi.IOut = -Emo_Ctrl.RefCurr << 14u;
        }
				else/* add else */
				{

				}
      }
    }
    else
    {
      if(Emo_Focpar_Cfg.EnableFrZero == 1u)
      {
        Emo_Foc.CountStart--;
      }
      else
      {
        Emo_Foc.CountStart = 0u;    /*Starttime for (Frequenz=0)  =0  */
      }

      Emo_Foc.StartFrequencySlope = 0;
    }

    /* jj => electrical rotation speed */
    jj = Mat_FixMulScale(Emo_Ctrl.Speedest, Emo_Ctrl.Factorspeed, Emo_Ctrl.Expspeedhigh);
    /* calculate mech. speed out of electrical speed (jj), jj not further used */
    Speed = jj / Emo_Foc.PolePair;
    /* Filter speed */
    Emo_Ctrl.ActSpeed = Mat_ExeLp_without_min_max(&Emo_Ctrl.SpeedLp, Speed);
		
#if (CURRENT_LIMIT_ESTIMAT) == 1u   /* set MinRefCurrent  2021-07-30-by zyg  */

		if(Emo_Status.MotorState == EMO_MOTOR_STATE_RUN)
		{		
			Emo_Ctrl.MinRefCurrent = Common_ExeLookUpTable((ADCResults.Voltage.Bat),
										((const s16 *)&(EMO_RefCurrentTableX[0u])),
										((const s16 *)&(EMO_RefCurrentTableY[0u])),
										RC_TABLE_LEN - 1u); /* Lookup table 2021-07-30-by zyg  */
			Emo_Ctrl.MaxRefCurrent = -Emo_Ctrl.MinRefCurrent;		
		}
#endif
  }
  else if (Emo_Status.MotorState == (EMO_MOTOR_STATE_RUN))
  {			
		Emo_CaculateIShunt();
		Emo_IShuntLimit();
		
		Emo_Ctrl.SpeedPi.PiMax = Emo_Ctrl.MaxRefCurrent;
		Emo_Ctrl.SpeedPi.PiMin = Emo_Ctrl.MinRefCurrent;

    /* jj => electrical rotation speed */
    jj = Mat_FixMulScale(Emo_Ctrl.Speedest, Emo_Ctrl.Factorspeed, Emo_Ctrl.Expspeedhigh);
    /* calculate mech. speed out of electrical speed (jj), jj not further used */
    Speed = jj / Emo_Foc.PolePair;
    /* Filter speed */
    Emo_Ctrl.ActSpeed = Mat_ExeLp_without_min_max(&Emo_Ctrl.SpeedLp, Speed);

    /* Speed Regulator: Execute PI algorithm for (imaginary) reference current */
    Emo_Ctrl.RefCurr = Mat_ExePi(&Emo_Ctrl.SpeedPi, Emo_Ctrl.RefSpeed - Emo_Ctrl.ActSpeed);
		
  }
  else
  {
    Emo_StopMotor();
  }
		
  /* read DC-Link-Voltage */
  Emo_Foc.DcLinkVoltage = ADC1->RES_OUT6.reg;
  Emo_Foc.Dcfactor1 = Emo_Foc.Kdcdivident1 / Emo_Foc.DcLinkVoltage;
  Emo_Foc.Dcfactor2 = __ssat(Mat_FixMulScale(Emo_Foc.DcLinkVoltage, Emo_Foc.Kdcfactor2, 3), MAT_FIX_SAT);
	
  Emo_Ctrl.ImagCurrPi.IMax = __ssat(Mat_FixMulScale(Emo_Foc.DcLinkVoltage, Emo_Foc.Kdcfactoriqc, 5u), MAT_FIX_SAT);
  Emo_Ctrl.ImagCurrPi.PiMax = Emo_Ctrl.ImagCurrPi.IMax;
  Emo_Ctrl.ImagCurrPi.IMin = -Emo_Ctrl.ImagCurrPi.IMax;
  Emo_Ctrl.ImagCurrPi.PiMin = Emo_Ctrl.ImagCurrPi.IMin;

  Emo_Ctrl.SpeedPi.IMin = Emo_Ctrl.SpeedPi.PiMin;
  Emo_Ctrl.SpeedPi.IMax = Emo_Ctrl.SpeedPi.PiMax;	

}

