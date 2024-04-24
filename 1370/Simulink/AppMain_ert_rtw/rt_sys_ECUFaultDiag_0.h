/*
 * File: rt_sys_ECUFaultDiag_0.h
 *
 * Code generated for Simulink model 'AppMain'.
 *
 * Model version                  : 5.33
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Mon Mar 27 10:05:33 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_rt_sys_ECUFaultDiag_0_h_
#define RTW_HEADER_rt_sys_ECUFaultDiag_0_h_
#ifndef AppMain_COMMON_INCLUDES_
#define AppMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AppMain_COMMON_INCLUDES_ */

#include "AppMain_types.h"

/* Block states (default storage) for system 'TemperaturState' (':3117') */
typedef struct
{
  uint16_T LowCnt;                     /* 'TemperaturState' (':3117') */
  uint16_T HighCnt;                    /* 'TemperaturState' (':3117') */
  uint16_T NormalCnt;                  /* 'TemperaturState' (':3117') */
  int8_T DefaultStatus;                /* 'TemperaturState' (':3117') */
  uint8_T is_active_c1_lib1;           /* 'TemperaturState' (':3117') */
  uint8_T is_c1_lib1;                  /* 'TemperaturState' (':3117') */
  boolean_T IsInited;                  /* 'TemperaturState' (':3117') */
}
DW_TemperaturState_T;

extern void TemperaturState_Init(int8_T *rty_Status, DW_TemperaturState_T
  *localDW);
extern void TemperaturState(int16_T rtu_Input, int8_T *rty_Status, uint16_T
  rtp_DelayCntTHR, int16_T rtp_HighEntryTHR, int16_T rtp_HighExitTHR, uint16_T
  rtp_LongDelayCntTHR, int16_T rtp_LowEntryTHR, int16_T rtp_LowExitTHR,
  DW_TemperaturState_T *localDW);

#endif                                 /* RTW_HEADER_rt_sys_ECUFaultDiag_0_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
