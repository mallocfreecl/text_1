/*
 * File: SleepMode.h
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

#ifndef RTW_HEADER_SleepMode_h_
#define RTW_HEADER_SleepMode_h_
#ifndef AppMain_COMMON_INCLUDES_
#define AppMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AppMain_COMMON_INCLUDES_ */

#include "AppMain_types.h"

/* Block states (default storage) for system 'SleepMode' (':6057') */
typedef struct
{
  uint16_T SleepWaitTime;              /* 'SleepMode' (':6059') */
  uint16_T SleepValidCnt;              /* 'SleepMode' (':6059') */
  uint8_T is_active_c5_AppMain;        /* 'SleepMode' (':6059') */
  uint8_T is_c5_AppMain;               /* 'SleepMode' (':6059') */
}
DW_SleepMode_T;

extern void SleepMode_Init(boolean_T *rty_GoToSleep, boolean_T *rty_ChipSleep,
  boolean_T *rty_ChipReset, DW_SleepMode_T *localDW);
extern void SleepMode(boolean_T rtu_SleepValid, boolean_T *rty_GoToSleep,
                      boolean_T *rty_ChipSleep, boolean_T *rty_ChipReset,
                      DW_SleepMode_T *localDW);

#endif                                 /* RTW_HEADER_SleepMode_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
