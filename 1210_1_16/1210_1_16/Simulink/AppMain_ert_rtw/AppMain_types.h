/*
 * File: AppMain_types.h
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

#ifndef RTW_HEADER_AppMain_types_h_
#define RTW_HEADER_AppMain_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_MotorFaultCnt_t_
#define DEFINED_TYPEDEF_FOR_MotorFaultCnt_t_

typedef struct
{
  uint8_T Short;
  uint8_T Open;
  uint8_T Lock;
  uint8_T DryRunning;
}
MotorFaultCnt_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_PwmInStatus_t_
#define DEFINED_TYPEDEF_FOR_PwmInStatus_t_

typedef struct
{
  boolean_T ShortGND;
  boolean_T ShortBAT;
  uint16_T Freq;
  uint16_T HighDuty;
}
PwmInStatus_t;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_T RT_MODEL_T;

#endif                                 /* RTW_HEADER_AppMain_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
