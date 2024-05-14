/*
 * File: SleepMode.c
 *
 * Code generated for Simulink model 'AppMain'.
 *
 * Model version                  : 5.192
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Thu Aug 18 11:09:12 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "SleepMode.h"

/* Include model header file for global data */
#include "AppMain.h"
#include "AppMain_private.h"

/* Named constants for Chart: ''SleepMode' (':6059')' */
#define IN_NO_ACTIVE_CHILD_hez5        ((uint8_T)0U)
#define IN_SleepValidFilter            ((uint8_T)1U)
#define IN_SleepWait                   ((uint8_T)2U)

/* System initialize for atomic system: 'SleepMode' (':6057') */
void SleepMode_Init(boolean_T *rty_GoToSleep, boolean_T *rty_ChipSleep,
                    boolean_T *rty_ChipReset, DW_SleepMode_T *localDW)
{
  /* SystemInitialize for Chart: 'SleepMode' (':6059') */
  localDW->is_active_c5_AppMain = 0U;
  localDW->is_c5_AppMain = IN_NO_ACTIVE_CHILD_hez5;
  localDW->SleepWaitTime = 0U;
  localDW->SleepValidCnt = 0U;
  *rty_GoToSleep = false;
  *rty_ChipSleep = false;
  *rty_ChipReset = false;
}

/* Output and update for atomic system: 'SleepMode' (':6057') */
void SleepMode(boolean_T rtu_SleepValid, boolean_T *rty_GoToSleep, boolean_T
               *rty_ChipSleep, boolean_T *rty_ChipReset, DW_SleepMode_T *localDW)
{
  uint32_T tmp;

  /* Chart: 'SleepMode' (':6059') incorporates:
   *  Constant: 'SleepModeValidCntTHR' (':6060')
   *  Constant: 'SleepModeWaitTimeTHR' (':6061')
   */
  /* Gateway: SleepMode/SleepMode */
  /* During: SleepMode/SleepMode */
  if ((uint32_T)localDW->is_active_c5_AppMain == 0U)
  {
    /* Entry: SleepMode/SleepMode */
    localDW->is_active_c5_AppMain = 1U;

    /* Entry Internal: SleepMode/SleepMode */
    /* Transition: (':6059:6') */
    localDW->is_c5_AppMain = IN_SleepValidFilter;

    /* Entry 'SleepValidFilter': (':6059:5') */
    *rty_GoToSleep = false;
    *rty_ChipSleep = false;
    *rty_ChipReset = false;
  }
  else if (localDW->is_c5_AppMain == IN_SleepValidFilter)
  {
    /* During 'SleepValidFilter': (':6059:5') */
    if (rtu_SleepValid && ((int32_T)localDW->SleepValidCnt >= 18))
    {
      /* Transition: (':6059:14') */
      localDW->SleepWaitTime = 1U;

      /* Exit 'SleepValidFilter': (':6059:5') */
      *rty_GoToSleep = true;
      localDW->is_c5_AppMain = IN_SleepWait;

      /* Transition: (':6059:10') */
    }
    else if (rtu_SleepValid)
    {
      /* Transition: (':6059:12') */
      tmp = (uint32_T)((uint32_T)localDW->SleepValidCnt + 1U);
      if ((uint32_T)((uint32_T)localDW->SleepValidCnt + 1U) > 65535U)
      {
        tmp = 65535U;
      }

      localDW->SleepValidCnt = (uint16_T)tmp;
    }
    else
    {
      /* Transition: (':6059:13') */
      localDW->SleepValidCnt = 0U;
    }

    /* During 'SleepWait': (':6059:7') */
    /* Transition: (':6059:17') */
  }
  else if ((int32_T)localDW->SleepWaitTime < 20)
  {
    /* Transition: (':6059:19') */
    tmp = (uint32_T)((uint32_T)localDW->SleepWaitTime + 1U);
    if ((uint32_T)((uint32_T)localDW->SleepWaitTime + 1U) > 65535U)
    {
      tmp = 65535U;
    }

    localDW->SleepWaitTime = (uint16_T)tmp;

    /* Transition: (':6059:20') */
  }
  else if (rtu_SleepValid)
  {
    /* Transition: (':6059:23') */
    *rty_ChipSleep = true;
  }
  else
  {
    /* Transition: (':6059:25') */
    *rty_ChipReset = true;
  }

  /* End of Chart: 'SleepMode' (':6059') */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
