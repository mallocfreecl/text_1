/*
 * File: PwmInParamEval.c
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

#include "PwmInParamEval.h"

/* Include model header file for global data */
#include "AppMain.h"
#include "AppMain_private.h"

/* Output and update for atomic system: 'PwmInshortDetect' (':5601') */
void PwmInshortDetect(void)
{
  boolean_T rtb_RelationalOperator;

  /* RelationalOperator: 'Relational Operator' (':5608') incorporates:
   *  Constant: 'PwmInBusTimeOutCntTHR' (':5607')
   *  Inport: 'PwmInIdleTime' (':5033')
   */
  rtb_RelationalOperator = ((int32_T)U.PwmInIdleTime >= 200);

  /* Outport: 'PwmInShortBat' (':6088') incorporates:
   *  Inport: 'PwmInPinSts' (':5029')
   *  Logic: 'Logical Operator' (':5604')
   */
  Y.PwmInShortBat = (rtb_RelationalOperator && U.PwmInPinSts);

  /* Outport: 'PwmInShortGND' (':6087') incorporates:
   *  Inport: 'PwmInPinSts' (':5029')
   *  Logic: 'Logical Operator1' (':5605')
   *  Logic: 'Logical Operator2' (':5606')
   */
  Y.PwmInShortGND = (rtb_RelationalOperator && (!U.PwmInPinSts));
}

/* Output and update for atomic system: 'PwmInParamEval' (':5595') */
void PwmInParamEval(void)
{
  /* SignalConversion generated from: 'PwmInStatus' (':5611') incorporates:
   *  Inport: 'PwmInFreq' (':5031')
   */
  B.Freq = U.PwmInFreq;

  /* SignalConversion generated from: 'PwmInStatus' (':5611') incorporates:
   *  Inport: 'PwmInDuty' (':5032')
   */
  B.HighDuty = U.PwmInDuty;

  /* Outputs for Atomic SubSystem: 'PwmInshortDetect' (':5601') */
  PwmInshortDetect();

  /* End of Outputs for SubSystem: 'PwmInshortDetect' (':5601') */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
