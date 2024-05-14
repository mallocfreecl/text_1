/*
 * File: ECUFaultDiag.h
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

#ifndef RTW_HEADER_ECUFaultDiag_h_
#define RTW_HEADER_ECUFaultDiag_h_
#ifndef AppMain_COMMON_INCLUDES_
#define AppMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AppMain_COMMON_INCLUDES_ */

#include "AppMain_types.h"

/* Child system includes */
#include "rt_sys_ECUFaultDiag_1.h"

/* Block states (default storage) for system 'AmbTemperatureDiag' (':5056') */
typedef struct
{
  DW_AmbTOverDiag_T sf_AmbTOverDiag;   /* 'AmbTOverDiag' (':5058') */
}
DW_AmbTemperatureDiag_T;

/* Block states (default storage) for system 'VoltageDiag' (':5063') */
typedef struct
{
  DW_AmbTOverDiag_T sf_VoltageDiag;    /* 'AmbTOverDiag' (':5058') */
}
DW_VoltageDiag_T;

/* Block states (default storage) for system 'ECUFaultDiag' (':5052') */
typedef struct
{
  DW_VoltageDiag_T VoltageDiag_farw;   /* 'VoltageDiag' (':5063') */
  DW_AmbTemperatureDiag_T AmbTemperatureDiag_gurd;/* 'AmbTemperatureDiag' (':5056') */
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
