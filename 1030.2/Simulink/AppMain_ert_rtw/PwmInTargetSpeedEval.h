/*
 * File: PwmInTargetSpeedEval.h
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

#ifndef RTW_HEADER_PwmInTargetSpeedEval_h_
#define RTW_HEADER_PwmInTargetSpeedEval_h_
#ifndef AppMain_COMMON_INCLUDES_
#define AppMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AppMain_COMMON_INCLUDES_ */

#include "AppMain_types.h"

/* Block signals for system 'DutyFilter' (':5951') */
typedef struct
{
  uint16_T Merge1;                     /* 'Merge1' (':5986') */
  uint16_T buf[6];                     /* 'DutyBufUpdate' (':5966') */
}
B_DutyFilter_T;

/* Block states (default storage) for system 'DutyFilter' (':5951') */
typedef struct
{
  uint16_T DutyBuf_PreviousInput[6];   /* 'DutyBuf' (':5956') */
  boolean_T DutyFilter_MODE;           /* 'DutyFilter' (':5951') */
}
DW_DutyFilter_T;

/* Block states (default storage) for system 'FreqValidJudge' (':5997') */
typedef struct
{
  uint16_T ValidCnt;                   /* 'FreqValidJudge' (':6002') */
  uint16_T InvalidCnt;                 /* 'FreqValidJudge' (':6002') */
  uint8_T is_active_c10_AppMain;       /* 'FreqValidJudge' (':6002') */
  uint8_T is_c10_AppMain;              /* 'FreqValidJudge' (':6002') */
}
DW_FreqValidJudge_T;

/* Block states (default storage) for system 'SpeedDelayOut' (':6029') */
typedef struct
{
  int16_T OutLast_PreviousInput;       /* 'OutLast' (':6034') */
  uint16_T DelayCnt;                   /* 'DelayOut' (':6033') */
  uint8_T is_active_c12_AppMain;       /* 'DelayOut' (':6033') */
  uint8_T is_c12_AppMain;              /* 'DelayOut' (':6033') */
}
DW_SpeedDelayOut_T;

extern void DutyBufUpdate(uint16_T rtu_data, uint16_T rty_buf[6]);
extern void BufDataSameFirstCnt(const uint16_T rtu_buf[6], uint16_T
  rtu_tolerance, uint16_T *rty_samecnt);
extern void DutyFilter_Init(B_DutyFilter_T *localB, DW_DutyFilter_T *localDW);
extern void DutyFilter_Reset(DW_DutyFilter_T *localDW);
extern void DutyFilter_Disable(DW_DutyFilter_T *localDW);
extern void DutyFilter(boolean_T rtu_Enable, uint16_T rtu_Duty, uint16_T
  rtu_DutyLast, boolean_T rtu_IsPwmDutyUpdated, uint16_T *rty_FilteredDuty,
  boolean_T *rty_Valid, B_DutyFilter_T *localB, DW_DutyFilter_T *localDW);
extern void FreqValidJudge_Init(boolean_T *rty_Valid, DW_FreqValidJudge_T
  *localDW);
extern void FreqValidJudge_Reset(boolean_T *rty_Valid, DW_FreqValidJudge_T
  *localDW);
extern boolean_T FreqValidJudge(uint16_T rtu_Freq, DW_FreqValidJudge_T *localDW);
extern void SpeedDelayOut_Init(int16_T *rty_Out, DW_SpeedDelayOut_T *localDW);
extern void SpeedDelayOut(int16_T rtu_In, uint16_T rtu_DelayTime, boolean_T
  rtu_Rst, int16_T *rty_Out, DW_SpeedDelayOut_T *localDW);
extern void PwmInShort_Init(void);
extern void PwmInShort_Reset(void);
extern void PwmInShort(void);
extern void PwmInNotShort_Init(void);
extern void PwmInNotShort_Reset(void);
extern void PwmInNotShort_Disable(void);
extern void PwmInNotShort(void);
extern void PwmInTargetSpeedEval_Init(void);
extern void PwmInTargetSpeedEval(void);

#endif                                 /* RTW_HEADER_PwmInTargetSpeedEval_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
