/*
 * File: PwmInTargetSpeedEval.h
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

#ifndef RTW_HEADER_PwmInTargetSpeedEval_h_
#define RTW_HEADER_PwmInTargetSpeedEval_h_
#ifndef AppMain_COMMON_INCLUDES_
#define AppMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AppMain_COMMON_INCLUDES_ */

/* Block signals for system 'DutyFilter' (':1703') */
typedef struct
{
  uint16_T Merge1;                     /* 'Merge1' (':2299') */
  uint16_T buf[6];                     /* 'DutyBufUpdate' (':2280') */
}
B_DutyFilter_T;

/* Block states (default storage) for system 'DutyFilter' (':1703') */
typedef struct
{
  uint16_T DelayInput1_DSTATE;         /* 'Delay Input1' (':3246:2') */
  uint16_T DutyBuf_PreviousInput[6];   /* 'DutyBuf' (':2275') */
  boolean_T DutyFilter_MODE;           /* 'DutyFilter' (':1703') */
}
DW_DutyFilter_T;

/* Block states (default storage) for system 'FreqValidJudge' (':1734') */
typedef struct
{
  uint16_T ValidCnt;                   /* 'FreqValidJudge' (':1737') */
  uint16_T InvalidCnt;                 /* 'FreqValidJudge' (':1737') */
  uint8_T is_active_c12_AppMain;       /* 'FreqValidJudge' (':1737') */
  uint8_T is_c12_AppMain;              /* 'FreqValidJudge' (':1737') */
}
DW_FreqValidJudge_T;

/* Block states (default storage) for system 'SpeedDelayOut' (':1394') */
typedef struct
{
  int16_T OutLast_PreviousInput;       /* 'OutLast' (':1399') */
  uint16_T DelayCnt;                   /* 'DelayOut' (':1398') */
  uint8_T is_active_c13_AppMain;       /* 'DelayOut' (':1398') */
  uint8_T is_c13_AppMain;              /* 'DelayOut' (':1398') */
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
extern void SpeedCurve(int32_T rtu_MaxSpeed, int16_T rtu_MinSpeed, uint16_T
  rtu_Duty, uint16_T rtu_SectorLast, int16_T *rty_Speed, uint16_T *rty_Delay,
  boolean_T *rty_Reset, uint16_T *rty_SectorNew);
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
extern void PwmInSpeedCtrl_Init(void);
extern void PwmInSpeedCtrl_Disable(void);
extern void PwmInSpeedCtrl(void);
extern void PwmInTargetSpeedEval_Init(void);
extern void PwmInTargetSpeedEval(void);

#endif                                 /* RTW_HEADER_PwmInTargetSpeedEval_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
