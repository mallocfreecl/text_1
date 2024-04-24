/*
 * File: rt_sys_ECUFaultDiag_0.h
 *
 * Code generated for Simulink model 'AppMain'.
 *
 * Model version                  : 9.0
 * Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
 * C/C++ source code generated on : Tue Mar  5 09:46:16 2024
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

/* Block states (default storage) for system 'AmbTOverDiag' (':3164') */
typedef struct
{
  uint16_T LowCnt;                     /* 'AmbTOverDiag' (':3164') */
  uint16_T HighCnt;                    /* 'AmbTOverDiag' (':3164') */
  uint16_T NormalCnt;                  /* 'AmbTOverDiag' (':3164') */
  int8_T DefaultStatus;                /* 'AmbTOverDiag' (':3164') */
  uint8_T is_active_c1_lib1;           /* 'AmbTOverDiag' (':3164') */
  uint8_T is_c1_lib1;                  /* 'AmbTOverDiag' (':3164') */
  boolean_T IsInited;                  /* 'AmbTOverDiag' (':3164') */
}
DW_AmbTOverDiag_T;

extern void AmbTOverDiag_Init(int8_T *rty_Status, DW_AmbTOverDiag_T *localDW);
extern void AmbTOverDiag(int16_T rtu_Input, int8_T *rty_Status, uint16_T
  rtp_DelayCntTHR, int16_T rtp_HighEntryTHR, int16_T rtp_HighExitTHR, uint16_T
  rtp_LongDelayCntTHR, int16_T rtp_LowEntryTHR, int16_T rtp_LowExitTHR,
  DW_AmbTOverDiag_T *localDW);

#endif                                 /* RTW_HEADER_rt_sys_ECUFaultDiag_0_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
