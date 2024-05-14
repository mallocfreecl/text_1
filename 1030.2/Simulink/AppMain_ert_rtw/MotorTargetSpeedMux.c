/*
 * File: MotorTargetSpeedMux.c
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

#include "MotorTargetSpeedMux.h"

/* Include model header file for global data */
#include "AppMain.h"
#include "AppMain_private.h"

/* Output and update for atomic system: 'MotorTargetSpeedMux' (':5585') */
int16_T MotorTargetSpeedMux(boolean_T rtu_GoToSleep, boolean_T
  rtu_TestModeSpeedCtrlEnable, int16_T rtu_TestModeSpeed, int16_T
  rtu_PwmInCtrlSpeed)
{
  int16_T rty_MotorTargetSpeed_0;

  /* Switch: 'Switch2' (':5592') incorporates:
   *  Constant: 'Constant' (':5590')
   *  Switch: 'Switch1' (':5591')
   */
  if (rtu_GoToSleep)
  {
    rty_MotorTargetSpeed_0 = 0;
  }
  else if (rtu_TestModeSpeedCtrlEnable)
  {
    /* Switch: 'Switch1' (':5591') */
    rty_MotorTargetSpeed_0 = rtu_TestModeSpeed;
  }
  else
  {
    rty_MotorTargetSpeed_0 = rtu_PwmInCtrlSpeed;
  }

  /* End of Switch: 'Switch2' (':5592') */
  return rty_MotorTargetSpeed_0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
