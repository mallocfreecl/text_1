/*
 * File: ECUFaultDiag.h
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

#ifndef RTW_HEADER_ECUFaultDiag_h_
#define RTW_HEADER_ECUFaultDiag_h_
#ifndef AppMain_COMMON_INCLUDES_
#define AppMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AppMain_COMMON_INCLUDES_ */

#include "rt_sys_ECUFaultDiag_0.h"

/* Block states (default storage) for system 'AmbTemperatureDiag' (':1039') */
typedef struct
{
  DW_AmbTOverDiag_T sf_AmbTOverDiag;   /* 'AmbTOverDiag' (':3164') */
}
DW_AmbTemperatureDiag_T;

/* Block states (default storage) for system 'VoltageDiag' (':797') */
typedef struct
{
  uint16_T LowCnt;                     /* 'VoltageDiag' (':1620') */
  uint16_T HighCnt;                    /* 'VoltageDiag' (':1620') */
  uint16_T NormalCnt;                  /* 'VoltageDiag' (':1620') */
  int8_T DefaultStatus;                /* 'VoltageDiag' (':1620') */
  uint8_T is_active_c2_lib1;           /* 'VoltageDiag' (':1620') */
  uint8_T is_c2_lib1;                  /* 'VoltageDiag' (':1620') */
  boolean_T IsInited;                  /* 'VoltageDiag' (':1620') */
}
DW_VoltageDiag_T;

/* Block states (default storage) for system 'ECUFaultDiag' (':769') */
typedef struct
{
  DW_VoltageDiag_T VoltageDiag_ospw;   /* 'VoltageDiag' (':797') */
  DW_AmbTemperatureDiag_T AmbTemperatureDiag_hnpz;/* 'AmbTemperatureDiag' (':1039') */
}
DW_ECUFaultDiag_T;

extern void AmbTemperatureDiag_Init(int8_T *rty_AmbTStatus,
  DW_AmbTemperatureDiag_T *localDW);
extern void AmbTemperatureDiag(int16_T rtu_AmbTemperature, int8_T
  *rty_AmbTStatus, DW_AmbTemperatureDiag_T *localDW);
extern void VoltageDiag_Init(int8_T *rty_VoltageStatus, DW_VoltageDiag_T
  *localDW);
extern void VoltageDiag(int16_T rtu_Voltage, int8_T *rty_VoltageStatus,
  DW_VoltageDiag_T *localDW);
extern void ECUFaultDiag_Init(int8_T *rty_VoltageStatus, int8_T *rty_AmbTStatus,
  DW_ECUFaultDiag_T *localDW);
extern void ECUFaultDiag(int16_T rtu_SupVoltage, int16_T rtu_AmbTemperature,
  boolean_T rtu_ChipFault, boolean_T *rty_ECUFault, int8_T *rty_VoltageStatus,
  int8_T *rty_AmbTStatus, DW_ECUFaultDiag_T *localDW);

#endif                                 /* RTW_HEADER_ECUFaultDiag_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
