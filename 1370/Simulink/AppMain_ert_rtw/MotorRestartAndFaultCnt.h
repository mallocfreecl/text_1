/*
 * File: MotorRestartAndFaultCnt.h
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

#ifndef RTW_HEADER_MotorRestartAndFaultCnt_h_
#define RTW_HEADER_MotorRestartAndFaultCnt_h_
#ifndef AppMain_COMMON_INCLUDES_
#define AppMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AppMain_COMMON_INCLUDES_ */

#include "AppMain_types.h"

/* Block signals for system 'MotorRestartAndFaultCnt' (':815') */
typedef struct
{
  int16_T Merge_jbei;                  /* 'Merge' (':2860') */
}
B_MotorRestartAndFaultCnt_T;

/* Block states (default storage) for system 'MotorRestartAndFaultCnt' (':815') */
typedef struct
{
  uint16_T RestartIntervalLast_PreviousInp;/* 'RestartIntervalLast' (':2133') */
  uint16_T Memory_PreviousInput;       /* 'Memory' (':2852') */
  uint16_T TimeCnt;                    /* 'MotorRestartFSM' (':871') */
  boolean_T DelayInput1_DSTATE_jdnj;   /* 'Delay Input1' (':2849:2') */
  int8_T If_ActiveSubsystem;           /* 'If' (':1833') */
  int8_T If_ActiveSubsystem_ghbp;      /* 'If' (':2851') */
  uint8_T ShortCntLast_PreviousInput;  /* 'ShortCntLast' (':834') */
  uint8_T OpenCntLast_PreviousInput;   /* 'OpenCntLast' (':2085') */
  uint8_T LockCntLast_PreviousInput;   /* 'LockCntLast' (':2086') */
  uint8_T is_active_c1_AppMain;        /* 'MotorRestartFSM' (':871') */
  uint8_T is_c1_AppMain;               /* 'MotorRestartFSM' (':871') */
}
DW_MotorRestartAndFaultCnt_T;

extern void Short(uint8_T rtu_FaultCnt, uint16_T *rty_Interval);
extern void MotorRestartAndFaultCnt_Init(B_MotorRestartAndFaultCnt_T *localB,
  DW_MotorRestartAndFaultCnt_T *localDW);
extern void MotorRestartAndFaultCnt(int16_T rtu_TargetSpeed, boolean_T
  rtu_ECUFault, boolean_T rtu_RestartComplete, boolean_T rtu_IsMotorShort,
  boolean_T rtu_IsMotorLock, boolean_T rtu_IsMotorOpenLoad, boolean_T
  rtu_IsMotorDryRunning, int16_T *rty_FaultSpeed, boolean_T *rty_FaultRestartEn,
  uint8_T *rty_ShortCnt, uint8_T *rty_OpenCnt, uint8_T *rty_LockCnt,
  B_MotorRestartAndFaultCnt_T *localB, DW_MotorRestartAndFaultCnt_T *localDW);

#endif                               /* RTW_HEADER_MotorRestartAndFaultCnt_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
