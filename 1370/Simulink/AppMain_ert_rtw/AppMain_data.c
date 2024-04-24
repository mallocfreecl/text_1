/*
 * File: AppMain_data.c
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

#include "AppMain.h"
#include "AppMain_private.h"

/* Constant parameters (default storage) */
const ConstP_T ConstP =
{
  /* Expression: int16([256 256 256])
   * Referenced by: '1-D Lookup Table' (':2249')
   */
  {
    256, 256, 256
  },

  /* Expression: int16([170 240 320])
   * Referenced by: '1-D Lookup Table' (':2249')
   */
  {
    170, 240, 320
  },

  /* Expression: [CtrlConst.FeedBack.LowTime.PwmInError,CtrlConst.FeedBack.LowTime.Voltage,CtrlConst.FeedBack.LowTime.AmbT,CtrlConst.FeedBack.LowTime.MotorShort,CtrlConst.FeedBack.LowTime.MotorOpenLoad,CtrlConst.FeedBack.LowTime.MotorLock,CtrlConst.FeedBack.LowTime.MotorDryRunning,CtrlConst.FeedBack.LowTime.Normal]

   * Referenced by: 'BusLowTimeTable' (':2376')
   */
  {
    0U, 25U, 10U, 15U, 15U, 15U, 10U, 5U
  },

  /* Expression: [CtrlConst.FeedBack.HighTime.PwmInError,CtrlConst.FeedBack.HighTime.Voltage,CtrlConst.FeedBack.HighTime.AmbT,CtrlConst.FeedBack.HighTime.MotorShort,CtrlConst.FeedBack.HighTime.MotorOpenLoad,CtrlConst.FeedBack.HighTime.MotorLock,CtrlConst.FeedBack.HighTime.MotorDryRunning,CtrlConst.FeedBack.HighTime.Normal]

   * Referenced by: 'BusHighTimeTable' (':2377')
   */
  {
    10U, 10U, 20U, 10U, 10U, 10U, 10U, 45U
  }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
