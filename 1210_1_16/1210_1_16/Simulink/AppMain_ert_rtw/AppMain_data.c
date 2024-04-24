/*
 * File: AppMain_data.c
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

#include "AppMain.h"

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
    0U, 25U, 20U, 15U, 15U, 15U, 10U, 5U
  },

  /* Expression: [CtrlConst.FeedBack.HighTime.PwmInError,CtrlConst.FeedBack.HighTime.Voltage,CtrlConst.FeedBack.HighTime.AmbT,CtrlConst.FeedBack.HighTime.MotorShort,CtrlConst.FeedBack.HighTime.MotorOpenLoad,CtrlConst.FeedBack.HighTime.MotorLock,CtrlConst.FeedBack.HighTime.MotorDryRunning,CtrlConst.FeedBack.HighTime.Normal]

   * Referenced by: 'BusHighTimeTable' (':2377')
   */
  {
    10U, 10U, 10U, 10U, 10U, 10U, 10U, 45U
  }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
