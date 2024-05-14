/*
 * File: ECUFaultDiag.c
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

#include "ECUFaultDiag.h"

/* Include model header file for global data */
#include "AppMain.h"
#include "AppMain_private.h"

/* System initialize for atomic system: 'AmbTemperatureDiag' (':5056') */
void AmbTemperatureDiag_Init(int8_T *rty_AmbTStatus, DW_AmbTemperatureDiag_T
  *localDW)
{
  /* SystemInitialize for Chart: 'AmbTOverDiag' (':5058') */
  AmbTOverDiag_Init(rty_AmbTStatus, &localDW->sf_AmbTOverDiag);
}

/* Output and update for atomic system: 'AmbTemperatureDiag' (':5056') */
void AmbTemperatureDiag(int16_T rtu_AmbTemperature, int8_T *rty_AmbTStatus,
  DW_AmbTemperatureDiag_T *localDW)
{
  /* Chart: 'AmbTOverDiag' (':5058') */
  AmbTOverDiag(rtu_AmbTemperature, rty_AmbTStatus, 10, 140, 135, -40, -38,
               &localDW->sf_AmbTOverDiag);
}

/* System initialize for atomic system: 'VoltageDiag' (':5063') */
void VoltageDiag_Init(int8_T *rty_VoltageStatus, DW_VoltageDiag_T *localDW)
{
  /* SystemInitialize for Chart: 'VoltageDiag' (':5065') */
  AmbTOverDiag_Init(rty_VoltageStatus, &localDW->sf_VoltageDiag);
}

/* Output and update for atomic system: 'VoltageDiag' (':5063') */
void VoltageDiag(int16_T rtu_Voltage, int8_T *rty_VoltageStatus,
                 DW_VoltageDiag_T *localDW)
{
  /* Chart: 'VoltageDiag' (':5065') */
  AmbTOverDiag(rtu_Voltage, rty_VoltageStatus, 10, 175, 170, 75, 82,
               &localDW->sf_VoltageDiag);
}

/* System initialize for atomic system: 'ECUFaultDiag' (':5052') */
void ECUFaultDiag_Init(int8_T *rty_VoltageStatus, int8_T *rty_AmbTStatus,
  DW_ECUFaultDiag_T *localDW)
{
  /* SystemInitialize for Atomic SubSystem: 'AmbTemperatureDiag' (':5056') */
  AmbTemperatureDiag_Init(rty_AmbTStatus, &localDW->AmbTemperatureDiag_gurd);

  /* End of SystemInitialize for SubSystem: 'AmbTemperatureDiag' (':5056') */

  /* SystemInitialize for Atomic SubSystem: 'VoltageDiag' (':5063') */
  VoltageDiag_Init(rty_VoltageStatus, &localDW->VoltageDiag_farw);

  /* End of SystemInitialize for SubSystem: 'VoltageDiag' (':5063') */
}

/* Output and update for atomic system: 'ECUFaultDiag' (':5052') */
void ECUFaultDiag(int16_T rtu_SupVoltage, int16_T rtu_AmbTemperature, boolean_T
                  rtu_ChipFault, boolean_T *rty_ECUFault, int8_T
                  *rty_VoltageStatus, int8_T *rty_AmbTStatus, DW_ECUFaultDiag_T *
                  localDW)
{
  /* Outputs for Atomic SubSystem: 'AmbTemperatureDiag' (':5056') */
  AmbTemperatureDiag(rtu_AmbTemperature, rty_AmbTStatus,
                     &localDW->AmbTemperatureDiag_gurd);

  /* End of Outputs for SubSystem: 'AmbTemperatureDiag' (':5056') */

  /* Outputs for Atomic SubSystem: 'VoltageDiag' (':5063') */
  VoltageDiag(rtu_SupVoltage, rty_VoltageStatus, &localDW->VoltageDiag_farw);

  /* End of Outputs for SubSystem: 'VoltageDiag' (':5063') */

  /* Logic: 'Logical Operator' (':5062') incorporates:
   *  Constant: 'Constant' (':5060:3')
   *  Constant: 'Constant' (':5061:3')
   *  RelationalOperator: 'Compare' (':5060:2')
   *  RelationalOperator: 'Compare' (':5061:2')
   */
  *rty_ECUFault = (((int32_T)*rty_VoltageStatus != 0) || ((int32_T)
    *rty_AmbTStatus != 0) || rtu_ChipFault);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
