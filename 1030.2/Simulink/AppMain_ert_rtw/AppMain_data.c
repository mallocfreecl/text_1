/*
 * File: AppMain_data.c
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

#include "AppMain.h"
#include "AppMain_private.h"

/* Constant parameters (default storage) */
const ConstP_T ConstP =
{
  /* Expression: int16([256 256 256])
   * Referenced by: '1-D Lookup Table' (':5650')
   */
  {
    256, 256, 256
  },

  /* Expression: int16([170 240 320])
   * Referenced by: '1-D Lookup Table' (':5650')
   */
  {
    170, 240, 320
  },

  /* Expression: CtrlConst.FeedBack.LowTime
   * Referenced by: 'LowTime' (':5151')
   */
  {
    35U, 35U, 30U, 25U, 20U, 15U, 5U
  },

  /* Expression: CtrlConst.FeedBack.HighTime
   * Referenced by: 'HighTime' (':5150')
   */
  {
    65U, 65U, 70U, 75U, 80U, 85U, 95U
  }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
