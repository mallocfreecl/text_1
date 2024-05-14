/*
 * File: AppMain_types.h
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

#ifndef RTW_HEADER_AppMain_types_h_
#define RTW_HEADER_AppMain_types_h_
#include "rtwtypes.h"

/* Model Code Variants */
#ifndef DEFINED_TYPEDEF_FOR_MotorFaultCnt_t_
#define DEFINED_TYPEDEF_FOR_MotorFaultCnt_t_

typedef struct
{
  uint8_T Short;
  uint8_T Open;
  uint8_T Lock;
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
