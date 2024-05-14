/*
 * File: AppMain.c
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

/* Block signals (default storage) */
B_T B;

/* Block states (default storage) */
DW_T DW;

/* External inputs (root inport signals with default storage) */
ExtU_T U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_T Y;

/* Real-time model */
static RT_MODEL_T M_;
RT_MODEL_T *const M = &M_;
static void rate_scheduler(void);
int16_T look1_is16lu32n16Ds32_binlcs(int16_T u0, const int16_T bp0[], const
  int16_T table[], uint32_T maxIndex)
{
  uint32_T frac;
  uint32_T iLeft;
  uint32_T iRght;
  int16_T bpLeftVar;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
     Rounding mode: 'simplest'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
     Rounding mode: 'simplest'
   */
  if ((int32_T)u0 <= (int32_T)bp0[0U])
  {
    iLeft = 0U;
    frac = 0U;
  }
  else if ((int32_T)u0 < (int32_T)bp0[maxIndex])
  {
    /* Binary Search */
    frac = (uint32_T)(maxIndex >> 1U);
    iLeft = 0U;
    iRght = maxIndex;
    while ((uint32_T)(iRght - iLeft) > 1U)
    {
      if ((int32_T)u0 < (int32_T)bp0[frac])
      {
        iRght = frac;
      }
      else
      {
        iLeft = frac;
      }

      frac = (uint32_T)((uint32_T)(iRght + iLeft) >> 1U);
    }

    bpLeftVar = bp0[iLeft];
    frac = (uint32_T)((uint32_T)((uint32_T)(uint16_T)(int32_T)((int32_T)u0 -
      (int32_T)bpLeftVar) << 16) / (uint32_T)(uint16_T)(int32_T)((int32_T)bp0
      [(uint32_T)(iLeft + 1U)] - (int32_T)bpLeftVar));
  }
  else
  {
    iLeft = (uint32_T)(maxIndex - 1U);
    frac = 65536U;
  }

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Rounding mode: 'simplest'
     Overflow mode: 'wrapping'
   */
  bpLeftVar = table[iLeft];
  return (int16_T)(int32_T)((int32_T)(int16_T)(int32_T)asr_s32((int32_T)
    ((int32_T)((int32_T)table[(uint32_T)(iLeft + 1U)] - (int32_T)bpLeftVar) *
     (int32_T)frac), 16U) + (int32_T)bpLeftVar);
}

int32_T div_nde_s32_floor(int32_T numerator, int32_T denominator)
{
  return (int32_T)((((numerator < 0) != (denominator < 0)) && (numerator %
    denominator != 0) ? -1 : 0) + (int32_T)(numerator / denominator));
}

int32_T asr_s32(int32_T u, uint32_T n)
{
  int32_T y;
  if (u >= 0)
  {
    y = (int32_T)(uint32_T)((uint32_T)u >> n);
  }
  else
  {
    y = (int32_T)((int32_T)-(int32_T)(uint32_T)((uint32_T)(int32_T)(-1 - u) >> n)
                  - 1);
  }

  return y;
}

/*
 *   This function updates active task flag for each subrate.
 * The function is called at model base rate, hence the
 * generated code self-manages all its subrates.
 */
static void rate_scheduler(void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (M->Timing.TaskCounters.TID[1])++;
  if ((M->Timing.TaskCounters.TID[1]) > 9)
  {                                    /* Sample time: [0.1s, 0.0s] */
    M->Timing.TaskCounters.TID[1] = 0;
  }
}

/* Model step function */
void AppMain_step(void)
{
  uint32_T qY;
  int16_T rtb_MotorTargetSpeed;
  int16_T rtb_Switch;
  uint16_T SectorNew;
  uint16_T x;
  uint16_T z;

  /* Outputs for Atomic SubSystem: 'PwmInParamEval' (':5595') */
  PwmInParamEval();

  /* End of Outputs for SubSystem: 'PwmInParamEval' (':5595') */
  if (M->Timing.TaskCounters.TID[1] == 0)
  {
    /* Outputs for Atomic SubSystem: 'SleepMode' (':6057') */
    /* Logic: 'Logical Operator' (':5327') incorporates:
     *  Inport: 'TestModeSleepEn' (':5042')
     *  Outport: 'PwmInShortBat' (':6088')
     */
    SleepMode(Y.PwmInShortBat && U.TestModeSleepEn, &B.GoToSleep, &B.ChipSleep,
              &B.ChipReset, &DW.SleepMode_h3wm);

    /* End of Outputs for SubSystem: 'SleepMode' (':6057') */

    /* Outport: 'ChipSleep' (':6077') */
    Y.ChipSleep = B.ChipSleep;

    /* Outport: 'ChipReset' (':6078') */
    Y.ChipReset = B.ChipReset;
  }

  /* RelationalOperator: 'Compare' (':5051:2') incorporates:
   *  Constant: 'Constant' (':5051:3')
   *  Inport: 'PwmInIdleTime' (':5033')
   */
  B.Compare = ((int32_T)U.PwmInIdleTime == 0);

  /* MATLAB Function: 'MATLAB Function' (':5047') incorporates:
   *  Inport: 'AnlogPinVoltage' (':5043')
   *  UnitDelay: 'Unit Delay' (':5048')
   */
  /* MATLAB Function 'AnlogVoltageToDuty/MATLAB Function': (':5047:1') */
  if ((int32_T)DW.UnitDelay_DSTATE == 0)
  {
    /* (':5047:1:3') */
    if ((int32_T)U.AnlogPinVoltage > 20)
    {
      /* (':5047:1:4') */
      /* (':5047:1:5') */
      SectorNew = 1U;
    }
    else
    {
      /* (':5047:1:7') */
      SectorNew = 0U;
    }
  }
  else if ((int32_T)U.AnlogPinVoltage <= 15)
  {
    /* (':5047:1:10') */
    /* (':5047:1:11') */
    SectorNew = 0U;
  }
  else
  {
    /* (':5047:1:13') */
    SectorNew = 1U;
  }

  if ((int32_T)SectorNew == 0)
  {
    /* (':5047:1:17') */
    /* (':5047:1:18') */
    B.Duty = 80U;
  }
  else if ((int32_T)U.AnlogPinVoltage <= 20)
  {
    /* (':5047:1:20') */
    /* (':5047:1:21') */
    B.Duty = 250U;
  }
  else if ((int32_T)U.AnlogPinVoltage >= 100)
  {
    /* (':5047:1:22') */
    /* (':5047:1:23') */
    B.Duty = 900U;
  }
  else
  {
    /* (':5047:1:25') */
    qY = (uint32_T)((uint32_T)U.AnlogPinVoltage - 20U);
    if ((uint32_T)((uint32_T)U.AnlogPinVoltage - 20U) > (uint32_T)
        U.AnlogPinVoltage)
    {
      qY = 0U;
    }

    qY *= 650U;
    if (qY > 65535U)
    {
      qY = 65535U;
    }

    z = (uint16_T)(uint32_T)((uint32_T)(uint16_T)qY / 80U);
    x = (uint16_T)(uint32_T)((uint32_T)(uint16_T)qY - (uint32_T)(uint16_T)
      (uint32_T)((uint32_T)z * 80U));
    if (((int32_T)x > 0) && ((int32_T)x >= 40))
    {
      z = (uint16_T)(int32_T)((int32_T)z + 1);
    }

    B.Duty = (uint16_T)(int32_T)((int32_T)z + 250);
  }

  /* Outputs for Atomic SubSystem: 'PwmInTargetSpeedEval' (':5612') */
  PwmInTargetSpeedEval();

  /* End of Outputs for SubSystem: 'PwmInTargetSpeedEval' (':5612') */

  /* Outputs for Atomic SubSystem: 'MotorTargetSpeedMux' (':5585') */
  /* Inport: 'TestModeSpeedCtrlEnable' (':5040') incorporates:
   *  Inport: 'TestModeSpeed' (':5041')
   */
  rtb_MotorTargetSpeed = MotorTargetSpeedMux(B.GoToSleep,
    U.TestModeSpeedCtrlEnable, U.TestModeSpeed, B.PwmInTargetSpeed);

  /* End of Outputs for SubSystem: 'MotorTargetSpeedMux' (':5585') */
  if (M->Timing.TaskCounters.TID[1] == 0)
  {
    /* Outputs for Atomic SubSystem: 'ECUFaultDiag' (':5052') */
    /* Inport: 'SupVoltage' (':5026') incorporates:
     *  Inport: 'AmbTemperature' (':5027')
     *  Inport: 'ChipFault' (':5028')
     */
    ECUFaultDiag(U.SupVoltage, U.AmbTemperature, U.ChipFault, &B.ECUFault,
                 &B.Status_mbnx, &B.Status_dotr, &DW.ECUFaultDiag_gulw);

    /* End of Outputs for SubSystem: 'ECUFaultDiag' (':5052') */
  }

  /* Outputs for Atomic SubSystem: 'MotorRestartAndFaultCnt' (':5328') */
  /* Outport: 'MotorRestartEnable' (':6076') incorporates:
   *  Inport: 'IsMotorDryRunning' (':5039')
   *  Inport: 'IsMotorLock' (':5035')
   *  Inport: 'IsMotorOpenLoad' (':5036')
   *  Inport: 'IsMotorShort' (':5034')
   *  Inport: 'RestartComplete' (':5037')
   *  Outport: 'LockCnt' (':6085')
   *  Outport: 'OpenCnt' (':6084')
   *  Outport: 'ShortCnt' (':6083')
   */
  MotorRestartAndFaultCnt(rtb_MotorTargetSpeed, B.ECUFault, U.RestartComplete,
    U.IsMotorShort, U.IsMotorLock, U.IsMotorOpenLoad, U.IsMotorDryRunning,
    &rtb_Switch, &Y.MotorRestartEnable, &Y.ShortCnt, &Y.OpenCnt, &Y.LockCnt,
    &DW.MotorRestartAndFaultCnt_j4bv);

  /* End of Outputs for SubSystem: 'MotorRestartAndFaultCnt' (':5328') */

  /* Outputs for Atomic SubSystem: 'SpeedLimit' (':6065') */
  /* Chart: 'AmbTPreOverDiag' (':6068') incorporates:
   *  Inport: 'AmbTemperature' (':5027')
   *  Outport: 'AmbTPreOverStatus' (':6089')
   */
  AmbTOverDiag(U.AmbTemperature, &Y.AmbTPreOverStatus, 100, 135, 130, -40, -38,
               &DW.sf_AmbTPreOverDiag);

  /* Switch: 'Switch' (':6071') incorporates:
   *  Outport: 'AmbTPreOverStatus' (':6089')
   */
  if ((int32_T)Y.AmbTPreOverStatus > 0)
  {
    /* Switch: 'Switch1' (':6072') incorporates:
     *  Constant: 'AmbTPreOverMaxSpeed' (':6069')
     *  RelationalOperator: 'Relational Operator' (':6070')
     */
    if (1270 < (int32_T)rtb_Switch)
    {
      /* Outport: 'TargetSpeedLimited' (':6080') */
      Y.TargetSpeedLimited = 1270;
    }
    else
    {
      /* Outport: 'TargetSpeedLimited' (':6080') */
      Y.TargetSpeedLimited = rtb_Switch;
    }

    /* End of Switch: 'Switch1' (':6072') */
  }
  else
  {
    /* Outport: 'TargetSpeedLimited' (':6080') */
    Y.TargetSpeedLimited = rtb_Switch;
  }

  /* End of Switch: 'Switch' (':6071') */
  /* End of Outputs for SubSystem: 'SpeedLimit' (':6065') */
  if (M->Timing.TaskCounters.TID[1] == 0)
  {
    /* Outputs for Atomic SubSystem: 'FeedBack' (':5070') */
    FeedBack();

    /* End of Outputs for SubSystem: 'FeedBack' (':5070') */

    /* Outport: 'FeedBackBusStatus' (':6079') */
    Y.FeedBackBusStatus = B.State;
  }

  if (M->Timing.TaskCounters.TID[1] == 0)
  {
    /* Outport: 'VoltageStatus' (':6081') */
    Y.VoltageStatus = B.Status_mbnx;

    /* Outport: 'AmbTStatus' (':6082') */
    Y.AmbTStatus = B.Status_dotr;
  }

  /* Outport: 'PwmInError' (':6086') */
  Y.PwmInError = B.PwmInError;

  /* Update for UnitDelay: 'Unit Delay' (':5048') incorporates:
   *  MATLAB Function: 'MATLAB Function' (':5047')
   */
  DW.UnitDelay_DSTATE = SectorNew;
  rate_scheduler();
}

/* Model initialize function */
void AppMain_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)M, 0,
                sizeof(RT_MODEL_T));

  /* block I/O */
  (void) memset(((void *) &B), 0,
                sizeof(B_T));

  /* states (dwork) */
  (void) memset((void *)&DW, 0,
                sizeof(DW_T));

  /* external inputs */
  (void)memset(&U, 0, sizeof(ExtU_T));

  /* external outputs */
  (void) memset((void *)&Y, 0,
                sizeof(ExtY_T));

  /* SystemInitialize for Atomic SubSystem: 'SleepMode' (':6057') */
  SleepMode_Init(&B.GoToSleep, &B.ChipSleep, &B.ChipReset, &DW.SleepMode_h3wm);

  /* End of SystemInitialize for SubSystem: 'SleepMode' (':6057') */

  /* SystemInitialize for Atomic SubSystem: 'PwmInTargetSpeedEval' (':5612') */
  PwmInTargetSpeedEval_Init();

  /* End of SystemInitialize for SubSystem: 'PwmInTargetSpeedEval' (':5612') */

  /* SystemInitialize for Atomic SubSystem: 'ECUFaultDiag' (':5052') */
  ECUFaultDiag_Init(&B.Status_mbnx, &B.Status_dotr, &DW.ECUFaultDiag_gulw);

  /* End of SystemInitialize for SubSystem: 'ECUFaultDiag' (':5052') */

  /* SystemInitialize for Atomic SubSystem: 'MotorRestartAndFaultCnt' (':5328') */
  MotorRestartAndFaultCnt_Init(&DW.MotorRestartAndFaultCnt_j4bv);

  /* End of SystemInitialize for SubSystem: 'MotorRestartAndFaultCnt' (':5328') */

  /* SystemInitialize for Atomic SubSystem: 'SpeedLimit' (':6065') */

  /* SystemInitialize for Chart: 'AmbTPreOverDiag' (':6068') incorporates:
   *  Outport: 'AmbTPreOverStatus' (':6089')
   */
  AmbTOverDiag_Init(&Y.AmbTPreOverStatus, &DW.sf_AmbTPreOverDiag);

  /* End of SystemInitialize for SubSystem: 'SpeedLimit' (':6065') */

  /* SystemInitialize for Atomic SubSystem: 'FeedBack' (':5070') */
  FeedBack_Init();

  /* End of SystemInitialize for SubSystem: 'FeedBack' (':5070') */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
