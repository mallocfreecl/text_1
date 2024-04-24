/*
 * File: MotorTargetSpeedMux.c
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

#include "MotorTargetSpeedMux.h"

/* Include model header file for global data */
#include "AppMain.h"
#include "AppMain_private.h"

/* Output and update for atomic system: 'MotorTargetSpeedMux' (':916') */
int16_T MotorTargetSpeedMux(boolean_T rtu_TestModeSpeedCtrlEnable, int16_T
  rtu_TestModeSpeed, int16_T rtu_PwmInCtrlSpeed)
{
  int16_T rty_MotorTargetSpeed_0;

  /* Switch: 'Switch1' (':923') */
  if (rtu_TestModeSpeedCtrlEnable)
  {
    /* Switch: 'Switch2' (':1569') */
    rty_MotorTargetSpeed_0 = rtu_TestModeSpeed;
  }
  else
  {
    /* Switch: 'Switch2' (':1569') */
    rty_MotorTargetSpeed_0 = rtu_PwmInCtrlSpeed;
  }

  /* End of Switch: 'Switch1' (':923') */
  return rty_MotorTargetSpeed_0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
