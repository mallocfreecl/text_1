/*
 * File: FeedBack.c
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

#include "FeedBack.h"

/* Include model header file for global data */
#include "AppMain.h"
#include "AppMain_private.h"

/* Named constants for Chart: ''DelayOut' (':2620:23:11')' */
#define IN_Delay                       ((uint8_T)1U)
#define IN_DirectOut                   ((uint8_T)2U)
#define IN_NO_ACTIVE_CHILD_j213        ((uint8_T)0U)
#define IN_Reset                       ((uint8_T)3U)

/* Named constants for Chart: ''StateKeep' (':2375')' */
#define IN_FalseState                  ((uint8_T)1U)
#define IN_NO_ACTIVE_CHILD_d5ea        ((uint8_T)0U)
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
  uint32_T tmp;
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
        tmp = (uint32_T)((uint32_T)localDW->DelayCnt + 1U);
        if ((uint32_T)((uint32_T)localDW->DelayCnt + 1U) > 65535U)
        {
          tmp = 65535U;
        }

        localDW->DelayCnt = (uint16_T)tmp;
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

  /* SystemInitialize for Chart: 'StateKeep' (':2375') */
  DW.is_active_c3_AppMain = 0U;
  DW.is_c3_AppMain = IN_NO_ACTIVE_CHILD_d5ea;
  DW.FalseTime = MAX_uint16_T;
  DW.TrueTime = MAX_uint16_T;
  B.State = false;
}

/* Output and update for atomic system: 'FeedBack' (':890') */
void FeedBack(void)
{
  uint32_T qY;
  uint16_T rtb_HighTime;
  uint16_T rtb_LowTime;
  uint16_T rtb_Switch6;
  boolean_T rtb_Compare_bqoi;
  boolean_T rtb_DataTypeConversion1;
  boolean_T rtb_DataTypeConversion1_bl05;
  boolean_T rtb_DataTypeConversion1_glzt;
  boolean_T rtb_LogicalOperator3;
  boolean_T rtb_LogicalOperator3_ew35;
  boolean_T rtb_LogicalOperator3_j3fr;

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
   *  Outport: 'VoltageStatus' (':2265')
   *  RelationalOperator: 'Compare' (':2347:2')
   */
  rtb_DataTypeConversion1_bl05 = FeedBackDelay(0, (int32_T)Y.VoltageStatus != 0,
    0, &B.FeedBackDelay4, &DW.FeedBackDelay4);

  /* End of Outputs for SubSystem: 'FeedBackDelay4' (':2346') */

  /* Outputs for Atomic SubSystem: 'FeedBackDelay5' (':2354') */
  /* Constant: 'Constant4' (':2355') incorporates:
   *  Constant: 'Constant5' (':2356')
   *  Constant: 'Constant' (':2353:3')
   *  Outport: 'AmbTStatus' (':2266')
   *  RelationalOperator: 'Compare' (':2353:2')
   */
  rtb_DataTypeConversion1 = FeedBackDelay(0, (int32_T)Y.AmbTStatus > 0, 0,
    &B.FeedBackDelay5, &DW.FeedBackDelay5);

  /* End of Outputs for SubSystem: 'FeedBackDelay5' (':2354') */

  /* Logic: 'Logical Operator3' (':3067') incorporates:
   *  Constant: 'Constant' (':3063:3')
   *  Inport: 'IsMotorShort' (':1826')
   *  Logic: 'Logical Operator1' (':3066')
   *  Memory: 'FeedBackEnLast' (':3064')
   *  Outport: 'ShortCnt' (':2584')
   *  RelationalOperator: 'Compare' (':3063:2')
   */
  rtb_LogicalOperator3 = ((U.IsMotorShort && DW.FeedBackEnLast_PreviousInput) ||
    ((int32_T)Y.ShortCnt >= 20));

  /* Logic: 'Logical Operator3' (':3077') incorporates:
   *  Constant: 'Constant' (':3073:3')
   *  Inport: 'IsMotorOpenLoad' (':1828')
   *  Logic: 'Logical Operator1' (':3076')
   *  Memory: 'FeedBackEnLast' (':3074')
   *  Outport: 'OpenCnt' (':2585')
   *  RelationalOperator: 'Compare' (':3073:2')
   */
  rtb_LogicalOperator3_ew35 = ((U.IsMotorOpenLoad &&
    DW.FeedBackEnLast_PreviousInp_nnxg) || ((int32_T)Y.OpenCnt >= 20));

  /* Logic: 'Logical Operator3' (':3046') incorporates:
   *  Constant: 'Constant' (':3042:3')
   *  Inport: 'IsMotorLock' (':1827')
   *  Logic: 'Logical Operator1' (':3045')
   *  Memory: 'FeedBackEnLast' (':3043')
   *  Outport: 'LockCnt' (':2586')
   *  RelationalOperator: 'Compare' (':3042:2')
   */
  rtb_LogicalOperator3_j3fr = ((U.IsMotorLock &&
    DW.FeedBackEnLast_PreviousInp_by2c) || ((int32_T)Y.LockCnt >= 20));

  /* Switch: 'Switch6' (':2618') incorporates:
   *  Constant: 'Constant3' (':2352')
   *  Switch: 'Switch' (':2348')
   *  Switch: 'Switch1' (':2357')
   */
  if (B.PwmInError)
  {
    rtb_Switch6 = 0U;
  }
  else if (rtb_DataTypeConversion1_bl05)
  {
    /* Switch: 'Switch' (':2348') incorporates:
     *  Bias: 'Bias' (':2403')
     *  Bias: 'Bias5' (':2619')
     *  Constant: 'Constant3' (':2352')
     */
    rtb_Switch6 = 1U;
  }
  else if (rtb_DataTypeConversion1)
  {
    /* Switch: 'Switch1' (':2357') incorporates:
     *  Switch: 'Switch' (':2348')
     */
    rtb_Switch6 = 2U;
  }
  else
  {
    /* RelationalOperator: 'Compare' (':2906:2') incorporates:
     *  Constant: 'Constant' (':2906:3')
     *  Switch: 'Switch' (':2348')
     *  Switch: 'Switch1' (':2357')
     */
    rtb_Compare_bqoi = ((int32_T)B.PwmInTargetSpeed != 0);

    /* Switch: 'Switch2' (':2360') incorporates:
     *  Bias: 'Bias1' (':2404')
     *  Bias: 'Bias2' (':2405')
     *  Inport: 'IsMotorDryRunning' (':2342')
     *  Inport: 'IsMotorLock' (':1827')
     *  Inport: 'IsMotorOpenLoad' (':1828')
     *  Inport: 'IsMotorShort' (':1826')
     *  Logic: 'Logical Operator' (':3065')
     *  Logic: 'Logical Operator' (':3075')
     *  Logic: 'Logical Operator' (':3044')
     *  Switch: 'Switch' (':2348')
     *  Switch: 'Switch1' (':2357')
     *  Switch: 'Switch3' (':2362')
     *  Switch: 'Switch4' (':2363')
     *  Switch: 'Switch5' (':2370')
     */
    if (U.IsMotorShort && rtb_LogicalOperator3 && rtb_Compare_bqoi)
    {
      rtb_Switch6 = 3U;
    }
    else if (U.IsMotorOpenLoad && rtb_LogicalOperator3_ew35 && rtb_Compare_bqoi)
    {
      /* Switch: 'Switch3' (':2362') */
      rtb_Switch6 = 4U;
    }
    else if (U.IsMotorLock && rtb_LogicalOperator3_j3fr && rtb_Compare_bqoi)
    {
      /* Switch: 'Switch4' (':2363') incorporates:
       *  Bias: 'Bias3' (':2406')
       *  Bias: 'Bias4' (':2407')
       *  Bias: 'Bias7' (':2616')
       *  Switch: 'Switch3' (':2362')
       *  Switch: 'Switch5' (':2370')
       */
      rtb_Switch6 = 5U;
    }
    else if (U.IsMotorDryRunning)
    {
      /* Switch: 'Switch5' (':2370') incorporates:
       *  Bias: 'Bias4' (':2407')
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
       *  Switch: 'Switch4' (':2363')
       *  Switch: 'Switch5' (':2370')
       */
      rtb_Switch6 = 7U;
    }

    /* End of Switch: 'Switch2' (':2360') */
  }

  /* End of Switch: 'Switch6' (':2618') */

  /* If: 'If' (':2645') incorporates:
   *  Inport: 'BusHighTime' (':2654')
   *  Inport: 'BusLowTime' (':2651')
   *  LookupNDDirect: 'BusHighTimeTable' (':2377')
   *  LookupNDDirect: 'BusLowTimeTable' (':2376')
   *  Switch: 'Switch6' (':2618')
   *
   * About 'BusHighTimeTable' (':2377'):
   *  1-dimensional Direct Look-Up returning a Scalar,
   *
   *     Remove protection against out-of-range input in generated code: 'off'
   *
   * About 'BusLowTimeTable' (':2376'):
   *  1-dimensional Direct Look-Up returning a Scalar,
   *
   *     Remove protection against out-of-range input in generated code: 'off'
   */
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
    rtb_LowTime = ConstP.BusLowTimeTable_table[(int32_T)rtb_Switch6];
    rtb_HighTime = ConstP.BusHighTimeTable_table[(int32_T)rtb_Switch6];

    /* End of Outputs for SubSystem: 'If Action Subsystem1' (':2650') */
  }

  /* End of If: 'If' (':2645') */

  /* Chart: 'StateKeep' (':2375') */
  /* Gateway: FeedBack/StateKeep */
  /* During: FeedBack/StateKeep */
  if ((uint32_T)DW.is_active_c3_AppMain == 0U)
  {
    /* Entry: FeedBack/StateKeep */
    DW.is_active_c3_AppMain = 1U;

    /* Entry Internal: FeedBack/StateKeep */
    /* Transition: (':2375:5') */
    DW.TrueTime = rtb_HighTime;
    DW.FalseTime = rtb_LowTime;
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
      DW.FalseTime = 0U;
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
        qY = (uint32_T)((uint32_T)DW.FalseTime - 1U);
        if ((uint32_T)((uint32_T)DW.FalseTime - 1U) > (uint32_T)DW.FalseTime)
        {
          qY = 0U;
        }

        DW.FalseTime = (uint16_T)qY;
      }
      else
      {
        /* Transition: (':2375:36') */
        if ((int32_T)DW.TrueTime == 0)
        {
          /* Transition: (':2375:38') */
          DW.TrueTime = rtb_HighTime;
          DW.FalseTime = rtb_LowTime;
        }
        else
        {
          /* Transition: (':2375:46') */
          /* Transition: (':2375:48') */
          /* Transition: (':2375:49') */
        }

        /* Transition: (':2375:39') */
      }
    }

    /* During 'TrueState': (':2375:4') */
  }
  else if (((int32_T)DW.TrueTime <= 1) && ((int32_T)rtb_LowTime > 0))
  {
    /* Transition: (':2375:14') */
    DW.is_c3_AppMain = IN_FalseState;

    /* Entry 'FalseState': (':2375:6') */
    B.State = false;
    DW.TrueTime = rtb_HighTime;
    DW.FalseTime = rtb_LowTime;
  }
  else
  {
    B.State = true;

    /* Transition: (':2375:50') */
    if ((int32_T)DW.TrueTime > 0)
    {
      /* Transition: (':2375:23') */
      qY = (uint32_T)((uint32_T)DW.TrueTime - 1U);
      if ((uint32_T)((uint32_T)DW.TrueTime - 1U) > (uint32_T)DW.TrueTime)
      {
        qY = 0U;
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

  /* Update for Memory: 'FeedBackEnLast' (':3064') */
  DW.FeedBackEnLast_PreviousInput = rtb_LogicalOperator3;

  /* Update for Memory: 'FeedBackEnLast' (':3074') */
  DW.FeedBackEnLast_PreviousInp_nnxg = rtb_LogicalOperator3_ew35;

  /* Update for Memory: 'FeedBackEnLast' (':3043') */
  DW.FeedBackEnLast_PreviousInp_by2c = rtb_LogicalOperator3_j3fr;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
