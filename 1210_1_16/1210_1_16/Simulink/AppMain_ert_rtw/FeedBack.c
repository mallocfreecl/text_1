/*
 * File: FeedBack.c
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

#include "rtwtypes.h"
#include "FeedBack.h"
#include "AppMain.h"
#include "AppMain_private.h"

/* Named constants for Chart: ''DelayOut' (':2620:23:11')' */
#define IN_Delay                       ((uint8_T)1U)
#define IN_DirectOut                   ((uint8_T)2U)
#define IN_NO_ACTIVE_CHILD_j213        ((uint8_T)0U)
#define IN_Reset                       ((uint8_T)3U)

/* Named constants for Chart: ''FeedBackTime' (':3199')' */
#define IN_FeedBackFiveAfter           ((uint8_T)1U)
#define IN_FeedBackFiveBefore          ((uint8_T)2U)
#define IN_NO_ACTIVE_CHILD_gl0o        ((uint8_T)0U)
#define IN_NewFaultJudge               ((uint8_T)3U)

/* Named constants for Chart: ''StateKeep' (':2375')' */
#define IN_FalseState                  ((uint8_T)1U)
#define IN_TrueState                   ((uint8_T)2U)

/*
 * System initialize for atomic system:
 *    'DelayOut' (':2620:23')
 *    'DelayOut' (':2346:23')
 *    'DelayOut' (':2354:23')
 */
void DelayOut_Init(int16_T *rty_Out, DW_DelayOut_T *localDW)
{
  /* SystemInitialize for Chart: 'DelayOut' (':2620:23:11') */
  localDW->is_active_c3_lib1 = 0U;
  localDW->is_c3_lib1 = IN_NO_ACTIVE_CHILD_j213;
  localDW->DelayCnt = 0U;
  *rty_Out = 0;
}

/*
 * Output and update for atomic system:
 *    'DelayOut' (':2620:23')
 *    'DelayOut' (':2346:23')
 *    'DelayOut' (':2354:23')
 */
void DelayOut(int16_T rtu_In, uint16_T rtu_DelayTime, int16_T *rty_Out,
              DW_DelayOut_T *localDW)
{
  boolean_T rtb_Rst_n2q0;

  /* Logic: 'Logical Operator' (':2620:23:14') incorporates:
   *  RelationalOperator: 'FixPt Relational Operator' (':2620:23:12:3')
   *  RelationalOperator: 'FixPt Relational Operator' (':2620:23:13:3')
   *  UnitDelay: 'Delay Input1' (':2620:23:12:2')
   *  UnitDelay: 'Delay Input1' (':2620:23:13:2')
   *
   * Block description for 'Delay Input1' (':2620:23:12:2'):
   *
   *  Store in Global RAM
   *
   * Block description for 'Delay Input1' (':2620:23:13:2'):
   *
   *  Store in Global RAM
   */
  rtb_Rst_n2q0 = ((rtu_DelayTime != localDW->DelayInput1_DSTATE_blit) ||
                  ((int32_T)rtu_In != (int32_T)localDW->DelayInput1_DSTATE));

  /* Chart: 'DelayOut' (':2620:23:11') incorporates:
   *  Memory: 'OutLast' (':2620:23:15')
   */
  /* Gateway: DelayOut/DelayOut */
  /* During: DelayOut/DelayOut */
  if ((uint32_T)localDW->is_active_c3_lib1 == 0U)
  {
    /* Entry: DelayOut/DelayOut */
    localDW->is_active_c3_lib1 = 1U;

    /* Entry Internal: DelayOut/DelayOut */
    /* Transition: (':11:27') */
    if ((int32_T)rtu_DelayTime == 0)
    {
      /* Transition: (':11:60') */
      localDW->is_c3_lib1 = IN_DirectOut;

      /* Entry 'DirectOut': (':11:46') */
      *rty_Out = rtu_In;
    }
    else
    {
      /* Transition: (':11:61') */
      /* Transition: (':11:57') */
      localDW->is_c3_lib1 = IN_Reset;

      /* Entry 'Reset': (':11:64') */
      localDW->DelayCnt = 2U;
      *rty_Out = localDW->OutLast_PreviousInput;
    }
  }
  else
  {
    switch (localDW->is_c3_lib1)
    {
     case IN_Delay:
      /* During 'Delay': (':11:70') */
      if ((int32_T)rtu_DelayTime == 0)
      {
        /* Transition: (':11:83') */
        /* Transition: (':11:82') */
        localDW->is_c3_lib1 = IN_DirectOut;

        /* Entry 'DirectOut': (':11:46') */
        *rty_Out = rtu_In;
      }
      else if (rtb_Rst_n2q0)
      {
        /* Transition: (':11:63') */
        localDW->is_c3_lib1 = IN_Reset;

        /* Entry 'Reset': (':11:64') */
        localDW->DelayCnt = 2U;
        *rty_Out = localDW->OutLast_PreviousInput;

        /* Transition: (':11:75') */
      }
      else if (localDW->DelayCnt < rtu_DelayTime)
      {
        /* Transition: (':11:76') */
        localDW->DelayCnt = (uint16_T)(uint32_T)((uint32_T)localDW->DelayCnt +
          1U);
        *rty_Out = localDW->OutLast_PreviousInput;
      }
      else
      {
        /* Transition: (':11:77') */
        /* Transition: (':11:79') */
        *rty_Out = rtu_In;

        /* Transition: (':11:78') */
      }
      break;

     case IN_DirectOut:
      /* During 'DirectOut': (':11:46') */
      if ((int32_T)rtu_DelayTime != 0)
      {
        /* Transition: (':11:84') */
        localDW->is_c3_lib1 = IN_Reset;

        /* Entry 'Reset': (':11:64') */
        localDW->DelayCnt = 2U;
        *rty_Out = localDW->OutLast_PreviousInput;
      }
      else
      {
        *rty_Out = rtu_In;
      }
      break;

     default:
      /* During 'Reset': (':11:64') */
      if ((int32_T)rtu_DelayTime == 0)
      {
        /* Transition: (':11:48') */
        localDW->is_c3_lib1 = IN_DirectOut;

        /* Entry 'DirectOut': (':11:46') */
        *rty_Out = rtu_In;
      }
      else if (!rtb_Rst_n2q0)
      {
        /* Transition: (':11:80') */
        localDW->is_c3_lib1 = IN_Delay;
      }
      else
      {
        /* Transition: (':11:68') */
        /* Transition: (':11:69') */
      }
      break;
    }
  }

  /* End of Chart: 'DelayOut' (':2620:23:11') */

  /* Update for UnitDelay: 'Delay Input1' (':2620:23:12:2')
   *
   * Block description for 'Delay Input1' (':2620:23:12:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE_blit = rtu_DelayTime;

  /* Update for UnitDelay: 'Delay Input1' (':2620:23:13:2')
   *
   * Block description for 'Delay Input1' (':2620:23:13:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE = rtu_In;

  /* Update for Memory: 'OutLast' (':2620:23:15') */
  localDW->OutLast_PreviousInput = *rty_Out;
}

/*
 * System initialize for atomic system:
 *    'PowerUpBlank' (':2620')
 *    'FeedBackDelay4' (':2346')
 *    'FeedBackDelay5' (':2354')
 */
void FeedBackDelay_Init(B_FeedBackDelay_T *localB, DW_FeedBackDelay_T *localDW)
{
  /* SystemInitialize for Atomic SubSystem: 'DelayOut' (':2620:23') */
  DelayOut_Init(&localB->Out, &localDW->DelayOut_fs2j);

  /* End of SystemInitialize for SubSystem: 'DelayOut' (':2620:23') */
}

/*
 * Output and update for atomic system:
 *    'PowerUpBlank' (':2620')
 *    'FeedBackDelay4' (':2346')
 *    'FeedBackDelay5' (':2354')
 */
boolean_T FeedBackDelay(uint16_T rtu_FeedBackStartDelayTime, boolean_T
  rtu_IsFault, uint16_T rtu_FeedBackEndDelayTime, B_FeedBackDelay_T *localB,
  DW_FeedBackDelay_T *localDW)
{
  boolean_T rty_FeedBackBusStatus_0;
  uint16_T rtb_DelayTime;

  /* Switch: 'Switch2' (':2620:28') incorporates:
   *  Memory: 'Memory' (':2620:26')
   *  RelationalOperator: 'FixPt Relational Operator' (':2620:24:3')
   *  RelationalOperator: 'FixPt Relational Operator' (':2620:25:3')
   *  Switch: 'Switch1' (':2620:27')
   *  UnitDelay: 'Delay Input1' (':2620:24:2')
   *  UnitDelay: 'Delay Input1' (':2620:25:2')
   *
   * Block description for 'Delay Input1' (':2620:24:2'):
   *
   *  Store in Global RAM
   *
   * Block description for 'Delay Input1' (':2620:25:2'):
   *
   *  Store in Global RAM
   */
  if ((int32_T)rtu_IsFault > (int32_T)localDW->DelayInput1_DSTATE)
  {
    rtb_DelayTime = rtu_FeedBackStartDelayTime;
  }
  else if ((int32_T)rtu_IsFault < (int32_T)localDW->DelayInput1_DSTATE_c3gy)
  {
    /* Switch: 'Switch1' (':2620:27') */
    rtb_DelayTime = rtu_FeedBackEndDelayTime;
  }
  else
  {
    rtb_DelayTime = localDW->Memory_PreviousInput;
  }

  /* End of Switch: 'Switch2' (':2620:28') */

  /* Outputs for Atomic SubSystem: 'DelayOut' (':2620:23') */
  /* DataTypeConversion: 'Data Type Conversion' (':2620:21') */
  DelayOut((int16_T)rtu_IsFault, rtb_DelayTime, &localB->Out,
           &localDW->DelayOut_fs2j);

  /* End of Outputs for SubSystem: 'DelayOut' (':2620:23') */

  /* DataTypeConversion: 'Data Type Conversion1' (':2620:22') */
  rty_FeedBackBusStatus_0 = ((int32_T)localB->Out != 0);

  /* Update for UnitDelay: 'Delay Input1' (':2620:25:2')
   *
   * Block description for 'Delay Input1' (':2620:25:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE = rtu_IsFault;

  /* Update for UnitDelay: 'Delay Input1' (':2620:24:2')
   *
   * Block description for 'Delay Input1' (':2620:24:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE_c3gy = rtu_IsFault;

  /* Update for Memory: 'Memory' (':2620:26') */
  localDW->Memory_PreviousInput = rtb_DelayTime;
  return rty_FeedBackBusStatus_0;
}

/* System initialize for atomic system: 'FeedBack' (':890') */
void FeedBack_Init(void)
{
  /* SystemInitialize for Atomic SubSystem: 'PowerUpBlank' (':2620') */
  FeedBackDelay_Init(&B.PowerUpBlank, &DW.PowerUpBlank);

  /* End of SystemInitialize for SubSystem: 'PowerUpBlank' (':2620') */

  /* SystemInitialize for Atomic SubSystem: 'FeedBackDelay4' (':2346') */
  FeedBackDelay_Init(&B.FeedBackDelay4, &DW.FeedBackDelay4);

  /* End of SystemInitialize for SubSystem: 'FeedBackDelay4' (':2346') */

  /* SystemInitialize for Atomic SubSystem: 'FeedBackDelay5' (':2354') */
  FeedBackDelay_Init(&B.FeedBackDelay5, &DW.FeedBackDelay5);

  /* End of SystemInitialize for SubSystem: 'FeedBackDelay5' (':2354') */

  /* SystemInitialize for Chart: 'FeedBackTime' (':3199') */
  DW.is_TimeJudge = IN_NO_ACTIVE_CHILD_gl0o;
  DW.is_active_c4_AppMain = 0U;
  DW.local_hightime = 0U;
  DW.local_lowtime = 0U;
  DW.FaultRecoverCnt = 0U;
  DW.FeedBackNumberLast = 0U;
  B.FalseKeepTime = 0U;
  B.TrueKeepTime = 0U;
  B.FaultRecoverCntOut = 0U;
  B.FeedBackFlagLast = false;
  B.FeedBackClearEn = false;

  /* SystemInitialize for Chart: 'StateKeep' (':2375') */
  DW.is_active_c3_AppMain = 0U;
  DW.is_c3_AppMain = IN_NO_ACTIVE_CHILD_gl0o;
  DW.FalseTime = MAX_uint16_T;
  DW.TrueTime = MAX_uint16_T;
  B.State = false;
}

/* Output and update for atomic system: 'FeedBack' (':890') */
void FeedBack(void)
{
  int32_T qY;
  uint32_T tmp;
  uint16_T rtb_HighTime;
  uint16_T rtb_LowTime;
  uint16_T rtb_Switch6;
  boolean_T rtb_Compare_bqoi;
  boolean_T rtb_DataTypeConversion1;
  boolean_T rtb_DataTypeConversion1_bl05;
  boolean_T rtb_DataTypeConversion1_glzt;

  /* Outputs for Atomic SubSystem: 'PowerUpBlank' (':2620') */
  /* Constant: 'Constant1' (':2622') incorporates:
   *  Constant: 'Constant' (':2621')
   *  Constant: 'Constant2' (':2624')
   */
  rtb_DataTypeConversion1_glzt = FeedBackDelay(0, true, 0, &B.PowerUpBlank,
    &DW.PowerUpBlank);

  /* End of Outputs for SubSystem: 'PowerUpBlank' (':2620') */

  /* Outputs for Atomic SubSystem: 'FeedBackDelay4' (':2346') */
  /* Constant: 'Constant2' (':2351') incorporates:
   *  Constant: 'Constant1' (':2350')
   *  Constant: 'Constant' (':2347:3')
   *  RelationalOperator: 'Compare' (':2347:2')
   */
  rtb_DataTypeConversion1_bl05 = FeedBackDelay(0, ((int32_T)B.Status_ila3 != 0),
    0, &B.FeedBackDelay4, &DW.FeedBackDelay4);

  /* End of Outputs for SubSystem: 'FeedBackDelay4' (':2346') */

  /* Outputs for Atomic SubSystem: 'FeedBackDelay5' (':2354') */
  /* Constant: 'Constant4' (':2355') incorporates:
   *  Constant: 'Constant5' (':2356')
   *  Constant: 'Constant' (':2353:3')
   *  RelationalOperator: 'Compare' (':2353:2')
   */
  rtb_DataTypeConversion1 = FeedBackDelay(0, ((int32_T)B.Status_mpxg > 0), 0,
    &B.FeedBackDelay5, &DW.FeedBackDelay5);

  /* End of Outputs for SubSystem: 'FeedBackDelay5' (':2354') */

  /* Switch: 'Switch6' (':2618') incorporates:
   *  Constant: 'Constant3' (':2352')
   */
  if (B.PwmInError)
  {
    rtb_Switch6 = 0U;
  }
  else
  {
    /* RelationalOperator: 'Compare' (':2906:2') incorporates:
     *  Constant: 'Constant' (':2906:3')
     */
    rtb_Compare_bqoi = ((int32_T)B.PwmInTargetSpeed != 0);

    /* Switch: 'Switch' (':2348') incorporates:
     *  Bias: 'Bias' (':2403')
     *  Bias: 'Bias5' (':2619')
     *  Constant: 'Constant3' (':2352')
     *  Constant: 'Constant' (':3063:3')
     *  Constant: 'Constant' (':3073:3')
     *  Constant: 'Constant' (':3042:3')
     *  Constant: 'Constant' (':3235:3')
     *  Inport: 'IsMotorDryRunning' (':2342')
     *  Inport: 'IsMotorLock' (':1827')
     *  Inport: 'IsMotorOpenLoad' (':1828')
     *  Inport: 'IsMotorShort' (':1826')
     *  Logic: 'Logical Operator' (':3244')
     *  Logic: 'Logical Operator' (':3245')
     *  Logic: 'Logical Operator' (':3065')
     *  Logic: 'Logical Operator3' (':3067')
     *  Logic: 'Logical Operator' (':3075')
     *  Logic: 'Logical Operator3' (':3077')
     *  Logic: 'Logical Operator' (':3044')
     *  Logic: 'Logical Operator3' (':3046')
     *  Logic: 'Logical Operator' (':3172')
     *  Logic: 'Logical Operator3' (':3236')
     *  Outport: 'DryRunningCnt' (':3162')
     *  Outport: 'LockCnt' (':2586')
     *  Outport: 'OpenCnt' (':2585')
     *  Outport: 'ShortCnt' (':2584')
     *  RelationalOperator: 'Compare' (':3063:2')
     *  RelationalOperator: 'Compare' (':3073:2')
     *  RelationalOperator: 'Compare' (':3042:2')
     *  RelationalOperator: 'Compare' (':3235:2')
     *  Switch: 'Switch1' (':2357')
     *  Switch: 'Switch2' (':2360')
     *  Switch: 'Switch3' (':2362')
     *  Switch: 'Switch4' (':2363')
     *  Switch: 'Switch5' (':2370')
     */
    if (rtb_DataTypeConversion1_bl05 && rtb_Compare_bqoi)
    {
      rtb_Switch6 = 1U;
    }
    else if (rtb_DataTypeConversion1 && rtb_Compare_bqoi)
    {
      /* Switch: 'Switch1' (':2357') */
      rtb_Switch6 = 2U;
    }
    else if (U.IsMotorShort && ((int32_T)Y.ShortCnt >= 2) && rtb_Compare_bqoi)
    {
      /* Switch: 'Switch2' (':2360') incorporates:
       *  Bias: 'Bias1' (':2404')
       *  Bias: 'Bias2' (':2405')
       *  Switch: 'Switch1' (':2357')
       */
      rtb_Switch6 = 3U;
    }
    else if (U.IsMotorOpenLoad && ((int32_T)Y.OpenCnt >= 2) && rtb_Compare_bqoi)
    {
      /* Switch: 'Switch3' (':2362') incorporates:
       *  Switch: 'Switch1' (':2357')
       *  Switch: 'Switch2' (':2360')
       */
      rtb_Switch6 = 4U;
    }
    else if (U.IsMotorLock && ((int32_T)Y.LockCnt >= 2) && rtb_Compare_bqoi)
    {
      /* Switch: 'Switch4' (':2363') incorporates:
       *  Bias: 'Bias3' (':2406')
       *  Bias: 'Bias4' (':2407')
       *  Bias: 'Bias7' (':2616')
       *  Switch: 'Switch1' (':2357')
       *  Switch: 'Switch2' (':2360')
       *  Switch: 'Switch3' (':2362')
       *  Switch: 'Switch5' (':2370')
       */
      rtb_Switch6 = 5U;
    }
    else if (U.IsMotorDryRunning && ((int32_T)Y.DryRunningCnt >= 2) &&
             rtb_Compare_bqoi)
    {
      /* Switch: 'Switch5' (':2370') incorporates:
       *  Bias: 'Bias4' (':2407')
       *  Switch: 'Switch1' (':2357')
       *  Switch: 'Switch2' (':2360')
       *  Switch: 'Switch3' (':2362')
       *  Switch: 'Switch4' (':2363')
       */
      rtb_Switch6 = 6U;
    }
    else
    {
      /* Switch: 'Switch3' (':2362') incorporates:
       *  Bias: 'Bias3' (':2406')
       *  Bias: 'Bias4' (':2407')
       *  Bias: 'Bias7' (':2616')
       *  Switch: 'Switch1' (':2357')
       *  Switch: 'Switch2' (':2360')
       *  Switch: 'Switch4' (':2363')
       *  Switch: 'Switch5' (':2370')
       */
      rtb_Switch6 = 7U;
    }

    /* End of Switch: 'Switch' (':2348') */
  }

  /* End of Switch: 'Switch6' (':2618') */

  /* If: 'If' (':2645') */
  if (!rtb_DataTypeConversion1_glzt)
  {
    /* Outputs for IfAction SubSystem: 'If Action Subsystem' (':2646') incorporates:
     *  ActionPort: 'Action Port' (':2648')
     */
    /* SignalConversion generated from: 'LowTime' (':2656') incorporates:
     *  Constant: 'Constant' (':2658')
     */
    rtb_LowTime = 0U;

    /* SignalConversion generated from: 'HighTime' (':2657') incorporates:
     *  Constant: 'Constant1' (':2661')
     */
    rtb_HighTime = 1U;

    /* End of Outputs for SubSystem: 'If Action Subsystem' (':2646') */
  }
  else
  {
    /* Outputs for IfAction SubSystem: 'If Action Subsystem1' (':2650') incorporates:
     *  ActionPort: 'Action Port' (':2652')
     */
    /* SignalConversion generated from: 'BusLowTime' (':2651') incorporates:
     *  LookupNDDirect: 'BusLowTimeTable' (':2376')
     *  Switch: 'Switch6' (':2618')
     *
     * About 'BusLowTimeTable' (':2376'):
     *  1-dimensional Direct Look-Up returning a Scalar,
     *
     *     Remove protection against out-of-range input in generated code: 'off'
     */
    rtb_LowTime = ConstP.BusLowTimeTable_table[(int32_T)rtb_Switch6];

    /* SignalConversion generated from: 'BusHighTime' (':2654') incorporates:
     *  LookupNDDirect: 'BusHighTimeTable' (':2377')
     *  Switch: 'Switch6' (':2618')
     *
     * About 'BusHighTimeTable' (':2377'):
     *  1-dimensional Direct Look-Up returning a Scalar,
     *
     *     Remove protection against out-of-range input in generated code: 'off'
     */
    rtb_HighTime = ConstP.BusHighTimeTable_table[(int32_T)rtb_Switch6];

    /* End of Outputs for SubSystem: 'If Action Subsystem1' (':2650') */
  }

  /* End of If: 'If' (':2645') */

  /* Logic: 'Logical Operator' (':3238') incorporates:
   *  Constant: 'Constant' (':3183:3')
   *  Constant: 'Constant' (':3239:3')
   *  RelationalOperator: 'Compare' (':3183:2')
   *  RelationalOperator: 'Compare' (':3239:2')
   */
  B.LogicalOperator = (((int32_T)rtb_Switch6 != 7) && ((int32_T)rtb_Switch6 != 0));

  /* Chart: 'FeedBackTime' (':3199') incorporates:
   *  Inport: 'FeedBackClearComplete' (':3230')
   *  Inport: 'FeedBackCnt' (':3211')
   */
  /* Gateway: FeedBack/FaultFeedBack/FeedBackTime */
  /* During: FeedBack/FaultFeedBack/FeedBackTime */
  if ((uint32_T)DW.is_active_c4_AppMain == 0U)
  {
    /* Entry: FeedBack/FaultFeedBack/FeedBackTime */
    DW.is_active_c4_AppMain = 1U;

    /* Entry Internal: FeedBack/FaultFeedBack/FeedBackTime */
    /* Transition: (':3199:84') */
    /* Entry 'TimeJudge': (':3199:60') */
    /* Entry Internal 'TimeJudge': (':3199:60') */
    /* Transition: (':3199:69') */
    if (((int32_T)U.FeedBackCnt < 5) && B.LogicalOperator)
    {
      /* Transition: (':3199:68') */
      DW.is_TimeJudge = IN_FeedBackFiveBefore;

      /* Entry 'FeedBackFiveBefore': (':3199:70') */
      DW.local_hightime = rtb_HighTime;
      DW.local_lowtime = rtb_LowTime;
      B.FeedBackFlagLast = true;
      DW.FaultRecoverCnt = 0U;
      DW.FeedBackNumberLast = rtb_Switch6;
    }
    else
    {
      /* Transition: (':3199:83') */
      DW.is_TimeJudge = IN_FeedBackFiveAfter;

      /* Entry 'FeedBackFiveAfter': (':3199:63') */
    }
  }
  else
  {
    /* During 'TimeJudge': (':3199:60') */
    switch (DW.is_TimeJudge)
    {
     case IN_FeedBackFiveAfter:
      /* During 'FeedBackFiveAfter': (':3199:63') */
      if (((int32_T)U.FeedBackCnt < 5) && B.LogicalOperator)
      {
        /* Transition: (':3199:61') */
        DW.is_TimeJudge = IN_FeedBackFiveBefore;

        /* Entry 'FeedBackFiveBefore': (':3199:70') */
        DW.local_hightime = rtb_HighTime;
        DW.local_lowtime = rtb_LowTime;
        B.FeedBackFlagLast = true;
        DW.FaultRecoverCnt = 0U;
        DW.FeedBackNumberLast = rtb_Switch6;
      }
      else
      {
        DW.local_hightime = rtb_HighTime;
        DW.local_lowtime = rtb_LowTime;
        B.FeedBackFlagLast = B.LogicalOperator;

        /* Transition: (':3199:66') */
        /* Transition: (':3199:67') */
        B.TrueKeepTime = DW.local_hightime;
        B.FalseKeepTime = DW.local_lowtime;
      }
      break;

     case IN_FeedBackFiveBefore:
      /* During 'FeedBackFiveBefore': (':3199:70') */
      if ((rtb_Switch6 != DW.FeedBackNumberLast) && B.LogicalOperator &&
          ((int32_T)U.FeedBackCnt >= 5) && ((int32_T)DW.FaultRecoverCnt >= 50))
      {
        /* Transition: (':3199:89') */
        DW.is_TimeJudge = IN_NewFaultJudge;

        /* Entry 'NewFaultJudge': (':3199:87') */
      }
      else if (((int32_T)U.FeedBackCnt >= 5) && ((int32_T)DW.FaultRecoverCnt >=
                50))
      {
        /* Transition: (':3199:82') */
        DW.is_TimeJudge = IN_FeedBackFiveAfter;

        /* Entry 'FeedBackFiveAfter': (':3199:63') */
      }
      else
      {
        /* Transition: (':3199:76') */
        if (rtb_Switch6 != DW.FeedBackNumberLast)
        {
          /* Transition: (':3199:75') */
          tmp = (uint32_T)((uint32_T)DW.FaultRecoverCnt + 1U);
          if ((uint32_T)((uint32_T)DW.FaultRecoverCnt + 1U) > 65535U)
          {
            tmp = 65535U;
          }

          DW.FaultRecoverCnt = (uint16_T)tmp;
        }
        else if ((int32_T)DW.FaultRecoverCnt > 0)
        {
          /* Transition: (':3199:77') */
          qY = (int32_T)(uint32_T)((uint32_T)DW.FaultRecoverCnt - 1U);
          if ((uint32_T)((uint32_T)DW.FaultRecoverCnt - 1U) > (uint32_T)
              DW.FaultRecoverCnt)
          {
            qY = 0;
          }

          DW.FaultRecoverCnt = (uint16_T)qY;
        }
        else
        {
          /* Transition: (':3199:81') */
        }

        if ((int32_T)DW.FaultRecoverCnt > 50)
        {
          /* Transition: (':3199:78') */
          DW.FaultRecoverCnt = 50U;
        }
        else
        {
          /* Transition: (':3199:80') */
        }

        /* Transition: (':3199:79') */
        B.TrueKeepTime = DW.local_hightime;
        B.FalseKeepTime = DW.local_lowtime;
        B.FaultRecoverCntOut = DW.FaultRecoverCnt;
        if ((int32_T)U.FeedBackClearComplete == 1)
        {
          /* Transition: (':3199:108') */
          B.FeedBackClearEn = false;
        }
        else
        {
          /* Transition: (':3199:109') */
        }
      }
      break;

     default:
      /* During 'NewFaultJudge': (':3199:87') */
      if (B.FeedBackClearEn)
      {
        /* Transition: (':3199:90') */
        DW.is_TimeJudge = IN_FeedBackFiveBefore;

        /* Entry 'FeedBackFiveBefore': (':3199:70') */
        DW.local_hightime = rtb_HighTime;
        DW.local_lowtime = rtb_LowTime;
        B.FeedBackFlagLast = B.LogicalOperator;
        DW.FaultRecoverCnt = 0U;
        DW.FeedBackNumberLast = rtb_Switch6;
      }
      else
      {
        B.FeedBackClearEn = true;
      }
      break;
    }
  }

  /* End of Chart: 'FeedBackTime' (':3199') */

  /* Chart: 'StateKeep' (':2375') */
  /* Gateway: FeedBack/StateKeep */
  /* During: FeedBack/StateKeep */
  if ((uint32_T)DW.is_active_c3_AppMain == 0U)
  {
    /* Entry: FeedBack/StateKeep */
    DW.is_active_c3_AppMain = 1U;

    /* Entry Internal: FeedBack/StateKeep */
    /* Transition: (':2375:5') */
    DW.TrueTime = B.TrueKeepTime;
    DW.FalseTime = B.FalseKeepTime;
    if ((int32_T)DW.FalseTime != 0)
    {
      /* Transition: (':2375:41') */
      DW.is_c3_AppMain = IN_FalseState;

      /* Entry 'FalseState': (':2375:6') */
      B.State = false;
    }
    else
    {
      /* Transition: (':2375:43') */
      /* Transition: (':2375:44') */
      DW.is_c3_AppMain = IN_TrueState;

      /* Entry 'TrueState': (':2375:4') */
      B.State = true;
    }
  }
  else if (DW.is_c3_AppMain == IN_FalseState)
  {
    /* During 'FalseState': (':2375:6') */
    if (((int32_T)DW.FalseTime <= 1) && ((int32_T)DW.TrueTime > 0))
    {
      /* Transition: (':2375:15') */
      DW.is_c3_AppMain = IN_TrueState;

      /* Entry 'TrueState': (':2375:4') */
      B.State = true;
      DW.FalseTime = 0U;
    }
    else
    {
      B.State = false;

      /* Transition: (':2375:51') */
      if ((int32_T)DW.FalseTime > 0)
      {
        /* Transition: (':2375:28') */
        qY = (int32_T)(uint32_T)((uint32_T)DW.FalseTime - 1U);
        if ((uint32_T)((uint32_T)DW.FalseTime - 1U) > (uint32_T)DW.FalseTime)
        {
          qY = 0;
        }

        DW.FalseTime = (uint16_T)qY;

        /* Transition: (':2375:36') */
      }
      else if ((int32_T)DW.TrueTime == 0)
      {
        /* Transition: (':2375:38') */
        DW.TrueTime = B.TrueKeepTime;
        DW.FalseTime = B.FalseKeepTime;
      }
      else
      {
        /* Transition: (':2375:46') */
        /* Transition: (':2375:48') */
        /* Transition: (':2375:49') */

        /* Transition: (':2375:39') */
      }
    }

    /* During 'TrueState': (':2375:4') */
  }
  else if (((int32_T)DW.TrueTime <= 1) && ((int32_T)B.FalseKeepTime > 0))
  {
    /* Transition: (':2375:14') */
    DW.is_c3_AppMain = IN_FalseState;

    /* Entry 'FalseState': (':2375:6') */
    B.State = false;
    DW.TrueTime = B.TrueKeepTime;
    DW.FalseTime = B.FalseKeepTime;
  }
  else
  {
    B.State = true;

    /* Transition: (':2375:50') */
    if ((int32_T)DW.TrueTime > 0)
    {
      /* Transition: (':2375:23') */
      qY = (int32_T)(uint32_T)((uint32_T)DW.TrueTime - 1U);
      if ((uint32_T)((uint32_T)DW.TrueTime - 1U) > (uint32_T)DW.TrueTime)
      {
        qY = 0;
      }

      DW.TrueTime = (uint16_T)qY;
    }
    else
    {
      /* Transition: (':2375:31') */
      /* Transition: (':2375:33') */
      /* Transition: (':2375:34') */
    }
  }

  /* End of Chart: 'StateKeep' (':2375') */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
