/*
 * File: rt_sys_ECUFaultDiag_1.h
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

#ifndef RTW_HEADER_rt_sys_ECUFaultDiag_1_h_
#define RTW_HEADER_rt_sys_ECUFaultDiag_1_h_
#ifndef AppMain_COMMON_INCLUDES_
#define AppMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AppMain_COMMON_INCLUDES_ */

#include "AppMain_types.h"

/* Block states (default storage) for system 'AmbTOverDiag' (':5058') */
typedef struct
{
  uint16_T LowCnt;                     /* 'AmbTOverDiag' (':5058') */
  uint16_T HighCnt;                    /* 'AmbTOverDiag' (':5058') */
  uint16_T NormalCnt;                  /* 'AmbTOverDiag' (':5058') */
  int8_T DefaultStatus;                /* 'AmbTOverDiag' (':5058') */
  uint8_T is_active_c2_lib1;           /* 'AmbTOverDiag' (':5058') */
  uint8_T is_c2_lib1;                  /* 'AmbTOverDiag' (':5058') */
  boolean_T IsInited;                  /* 'AmbTOverDiag' (':5058') */
}
DW_AmbTOverDiag_T;

extern void AmbTOverDiag_Init(int8_T *rty_Status, DW_AmbTOverDiag_T *localDW);
extern void AmbTOverDiag(int16_T rtu_Input, int8_T *rty_Status, uint16_T
  rtp_DelayCntTHR, int16_T rtp_HighEntryTHR, int16_T rtp_HighExitTHR, int16_T
  rtp_LowEntryTHR, int16_T rtp_LowExitTHR, DW_AmbTOverDiag_T *localDW);

#endif                                 /* RTW_HEADER_rt_sys_ECUFaultDiag_1_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
