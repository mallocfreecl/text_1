/*
 * File: FeedBack.c
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

#include "FeedBack.h"

/* Include model header file for global data */
#include "AppMain.h"
#include "AppMain_private.h"

/* Named constants for Chart: ''DelayOut' (':5103:23:11')' */
#define IN_Delay                       ((uint8_T)1U)
#define IN_DirectOut                   ((uint8_T)2U)
#define IN_NO_ACTIVE_CHILD_boug        ((uint8_T)0U)
#define IN_Reset                       ((uint8_T)3U)

/* Named constants for Chart: ''DelayOut' (':5218:23:11')' */
#define IN_Delay_inpy                  ((uint8_T)1U)
#define IN_DirectOut_cwoh              ((uint8_T)2U)
#define IN_NO_ACTIVE_CHILD_gyqk        ((uint8_T)0U)
#define IN_Reset_bc0f                  ((uint8_T)3U)

/* Named constants for Chart: ''StateKeep' (':5106')' */
#define IN_FalseState                  ((uint8_T)1U)
#define IN_NO_ACTIVE_CHILD_jx0i        ((uint8_T)0U)
#define IN_TrueState                   ((uint8_T)2U)

/*
 * System initialize for atomic system:
 *    'DelayOut' (':5103:23')
 *    'DelayOut' (':5191:23')
 *    'DelayOut' (':5200:23')
 *    'DelayOut' (':5209:23')
 */
void DelayOut_Init(int16_T *rty_Out, DW_DelayOut_T *localDW)
{
  /* SystemInitialize for Chart: 'DelayOut' (':5103:23:11') */
  localDW->is_active_c3_lib1 = 0U;
  localDW->is_c3_lib1 = IN_NO_ACTIVE_CHILD_boug;
  localDW->DelayCnt = 0U;
  *rty_Out = 0;
}

/*
 * Output and update for atomic system:
 *    'DelayOut' (':5103:23')
 *    'DelayOut' (':5191:23')
 *    'DelayOut' (':5200:23')
 *    'DelayOut' (':5209:23')
 */
void DelayOut(int16_T rtu_In, uint16_T rtu_DelayTime, int16_T *rty_Out,
              DW_DelayOut_T *localDW)
{
  uint32_T tmp;
  boolean_T rtb_Rst_meht;

  /* Logic: 'Logical Operator' (':5103:23:14') incorporates:
   *  RelationalOperator: 'FixPt Relational Operator' (':5103:23:12:3')
   *  RelationalOperator: 'FixPt Relational Operator' (':5103:23:13:3')
   *  UnitDelay: 'Delay Input1' (':5103:23:12:2')
   *  UnitDelay: 'Delay Input1' (':5103:23:13:2')
   *
   * Block description for 'Delay Input1' (':5103:23:12:2'):
   *
   *  Store in Global RAM
   *
   * Block description for 'Delay Input1' (':5103:23:13:2'):
   *
   *  Store in Global RAM
   */
  rtb_Rst_meht = ((rtu_DelayTime != localDW->DelayInput1_DSTATE_onhg) ||
                  ((int32_T)rtu_In != (int32_T)localDW->DelayInput1_DSTATE));

  /* Chart: 'DelayOut' (':5103:23:11') incorporates:
   *  Memory: 'OutLast' (':5103:23:15')
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
      else if (rtb_Rst_meht)
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
      else if (!rtb_Rst_meht)
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

  /* End of Chart: 'DelayOut' (':5103:23:11') */

  /* Update for UnitDelay: 'Delay Input1' (':5103:23:12:2')
   *
   * Block description for 'Delay Input1' (':5103:23:12:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE_onhg = rtu_DelayTime;

  /* Update for UnitDelay: 'Delay Input1' (':5103:23:13:2')
   *
   * Block description for 'Delay Input1' (':5103:23:13:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE = rtu_In;

  /* Update for Memory: 'OutLast' (':5103:23:15') */
  localDW->OutLast_PreviousInput = *rty_Out;
}

/*
 * System initialize for atomic system:
 *    'PowerUpBlank' (':5103')
 *    'FeedBackDelay4' (':5191')
 *    'FeedBackDelay4' (':5200')
 *    'FeedBackDelay4' (':5209')
 */
void FeedBackDelay_Init(B_FeedBackDelay_T *localB, DW_FeedBackDelay_T *localDW)
{
  /* SystemInitialize for Atomic SubSystem: 'DelayOut' (':5103:23') */
  DelayOut_Init(&localB->Out, &localDW->DelayOut_b34q);

  /* End of SystemInitialize for SubSystem: 'DelayOut' (':5103:23') */
}

/*
 * Output and update for atomic system:
 *    'PowerUpBlank' (':5103')
 *    'FeedBackDelay4' (':5191')
 *    'FeedBackDelay4' (':5200')
 *    'FeedBackDelay4' (':5209')
 */
boolean_T FeedBackDelay(uint16_T rtu_FeedBackStartDelayTime, boolean_T
  rtu_IsFault, uint16_T rtu_FeedBackEndDelayTime, B_FeedBackDelay_T *localB,
  DW_FeedBackDelay_T *localDW)
{
  boolean_T rty_FeedBackBusStatus_0;
  uint16_T rtb_DelayTime;

  /* Switch: 'Switch2' (':5103:28') incorporates:
   *  Memory: 'Memory' (':5103:26')
   *  RelationalOperator: 'FixPt Relational Operator' (':5103:24:3')
   *  RelationalOperator: 'FixPt Relational Operator' (':5103:25:3')
   *  Switch: 'Switch1' (':5103:27')
   *  UnitDelay: 'Delay Input1' (':5103:24:2')
   *  UnitDelay: 'Delay Input1' (':5103:25:2')
   *
   * Block description for 'Delay Input1' (':5103:24:2'):
   *
   *  Store in Global RAM
   *
   * Block description for 'Delay Input1' (':5103:25:2'):
   *
   *  Store in Global RAM
   */
  if ((int32_T)rtu_IsFault > (int32_T)localDW->DelayInput1_DSTATE)
  {
    rtb_DelayTime = rtu_FeedBackStartDelayTime;
  }
  else if ((int32_T)rtu_IsFault < (int32_T)localDW->DelayInput1_DSTATE_l1yc)
  {
    /* Switch: 'Switch1' (':5103:27') */
    rtb_DelayTime = rtu_FeedBackEndDelayTime;
  }
  else
  {
    rtb_DelayTime = localDW->Memory_PreviousInput;
  }

  /* End of Switch: 'Switch2' (':5103:28') */

  /* Outputs for Atomic SubSystem: 'DelayOut' (':5103:23') */
  /* DataTypeConversion: 'Data Type Conversion' (':5103:21') */
  DelayOut((int16_T)rtu_IsFault, rtb_DelayTime, &localB->Out,
           &localDW->DelayOut_b34q);

  /* End of Outputs for SubSystem: 'DelayOut' (':5103:23') */

  /* DataTypeConversion: 'Data Type Conversion1' (':5103:22') */
  rty_FeedBackBusStatus_0 = ((int32_T)localB->Out != 0);

  /* Update for UnitDelay: 'Delay Input1' (':5103:25:2')
   *
   * Block description for 'Delay Input1' (':5103:25:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE = rtu_IsFault;

  /* Update for UnitDelay: 'Delay Input1' (':5103:24:2')
   *
   * Block description for 'Delay Input1' (':5103:24:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE_l1yc = rtu_IsFault;

  /* Update for Memory: 'Memory' (':5103:26') */
  localDW->Memory_PreviousInput = rtb_DelayTime;
  return rty_FeedBackBusStatus_0;
}

/*
 * System initialize for atomic system:
 *    'DelayOut' (':5218:23')
 *    'DelayOut' (':5227:23')
 *    'DelayOut' (':5236:23')
 */
void DelayOut_pw2c_Init(int16_T *rty_Out, DW_DelayOut_ahel_T *localDW)
{
  /* SystemInitialize for Chart: 'DelayOut' (':5218:23:11') */
  localDW->is_active_c3_lib1 = 0U;
  localDW->is_c3_lib1 = IN_NO_ACTIVE_CHILD_gyqk;
  localDW->DelayCnt = 0U;
  *rty_Out = 0;
}

/*
 * Output and update for atomic system:
 *    'DelayOut' (':5218:23')
 *    'DelayOut' (':5227:23')
 *    'DelayOut' (':5236:23')
 */
void DelayOut_kj4v(int16_T rtu_In, uint16_T rtu_DelayTime, int16_T *rty_Out,
                   DW_DelayOut_ahel_T *localDW)
{
  uint32_T tmp;
  boolean_T rtb_Rst_f5xf;

  /* Logic: 'Logical Operator' (':5218:23:14') incorporates:
   *  RelationalOperator: 'FixPt Relational Operator' (':5218:23:12:3')
   *  RelationalOperator: 'FixPt Relational Operator' (':5218:23:13:3')
   *  UnitDelay: 'Delay Input1' (':5218:23:12:2')
   *  UnitDelay: 'Delay Input1' (':5218:23:13:2')
   *
   * Block description for 'Delay Input1' (':5218:23:12:2'):
   *
   *  Store in Global RAM
   *
   * Block description for 'Delay Input1' (':5218:23:13:2'):
   *
   *  Store in Global RAM
   */
  rtb_Rst_f5xf = ((rtu_DelayTime != localDW->DelayInput1_DSTATE_fqr4) ||
                  ((int32_T)rtu_In != (int32_T)localDW->DelayInput1_DSTATE));

  /* Chart: 'DelayOut' (':5218:23:11') incorporates:
   *  Memory: 'OutLast' (':5218:23:15')
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
      localDW->is_c3_lib1 = IN_DirectOut_cwoh;

      /* Entry 'DirectOut': (':11:46') */
      *rty_Out = rtu_In;
    }
    else
    {
      /* Transition: (':11:61') */
      /* Transition: (':11:57') */
      localDW->is_c3_lib1 = IN_Reset_bc0f;

      /* Entry 'Reset': (':11:64') */
      localDW->DelayCnt = 2U;
      *rty_Out = localDW->OutLast_PreviousInput;
    }
  }
  else
  {
    switch (localDW->is_c3_lib1)
    {
     case IN_Delay_inpy:
      /* During 'Delay': (':11:70') */
      if ((int32_T)rtu_DelayTime == 0)
      {
        /* Transition: (':11:83') */
        /* Transition: (':11:82') */
        localDW->is_c3_lib1 = IN_DirectOut_cwoh;

        /* Entry 'DirectOut': (':11:46') */
        *rty_Out = rtu_In;
      }
      else if (rtb_Rst_f5xf)
      {
        /* Transition: (':11:63') */
        localDW->is_c3_lib1 = IN_Reset_bc0f;

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

     case IN_DirectOut_cwoh:
      /* During 'DirectOut': (':11:46') */
      if ((int32_T)rtu_DelayTime != 0)
      {
        /* Transition: (':11:84') */
        localDW->is_c3_lib1 = IN_Reset_bc0f;

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
        localDW->is_c3_lib1 = IN_DirectOut_cwoh;

        /* Entry 'DirectOut': (':11:46') */
        *rty_Out = rtu_In;
      }
      else if (!rtb_Rst_f5xf)
      {
        /* Transition: (':11:80') */
        localDW->is_c3_lib1 = IN_Delay_inpy;
      }
      else
      {
        /* Transition: (':11:68') */
        /* Transition: (':11:69') */
      }
      break;
    }
  }

  /* End of Chart: 'DelayOut' (':5218:23:11') */

  /* Update for UnitDelay: 'Delay Input1' (':5218:23:12:2')
   *
   * Block description for 'Delay Input1' (':5218:23:12:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE_fqr4 = rtu_DelayTime;

  /* Update for UnitDelay: 'Delay Input1' (':5218:23:13:2')
   *
   * Block description for 'Delay Input1' (':5218:23:13:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE = rtu_In;

  /* Update for Memory: 'OutLast' (':5218:23:15') */
  localDW->OutLast_PreviousInput = *rty_Out;
}

/*
 * System initialize for atomic system:
 *    'FeedBackDelay4' (':5218')
 *    'FeedBackDelay4' (':5227')
 *    'FeedBackDelay4' (':5236')
 */
void FeedBackDelay4_Init(B_FeedBackDelay_fu1b_T *localB, DW_FeedBackDelay_lh44_T
  *localDW)
{
  /* SystemInitialize for Atomic SubSystem: 'DelayOut' (':5218:23') */
  DelayOut_pw2c_Init(&localB->Out, &localDW->DelayOut_kj4v5);

  /* End of SystemInitialize for SubSystem: 'DelayOut' (':5218:23') */
}

/*
 * Output and update for atomic system:
 *    'FeedBackDelay4' (':5218')
 *    'FeedBackDelay4' (':5227')
 *    'FeedBackDelay4' (':5236')
 */
boolean_T FeedBackDelay4(uint16_T rtu_FeedBackStartDelayTime, boolean_T
  rtu_IsFault, uint16_T rtu_FeedBackEndDelayTime, B_FeedBackDelay_fu1b_T *localB,
  DW_FeedBackDelay_lh44_T *localDW)
{
  boolean_T rty_FeedBackBusStatus_0;
  uint16_T rtb_DelayTime;

  /* Switch: 'Switch2' (':5218:28') incorporates:
   *  Memory: 'Memory' (':5218:26')
   *  RelationalOperator: 'FixPt Relational Operator' (':5218:24:3')
   *  RelationalOperator: 'FixPt Relational Operator' (':5218:25:3')
   *  Switch: 'Switch1' (':5218:27')
   *  UnitDelay: 'Delay Input1' (':5218:24:2')
   *  UnitDelay: 'Delay Input1' (':5218:25:2')
   *
   * Block description for 'Delay Input1' (':5218:24:2'):
   *
   *  Store in Global RAM
   *
   * Block description for 'Delay Input1' (':5218:25:2'):
   *
   *  Store in Global RAM
   */
  if ((int32_T)rtu_IsFault > (int32_T)localDW->DelayInput1_DSTATE)
  {
    rtb_DelayTime = rtu_FeedBackStartDelayTime;
  }
  else if ((int32_T)rtu_IsFault < (int32_T)localDW->DelayInput1_DSTATE_evfm)
  {
    /* Switch: 'Switch1' (':5218:27') */
    rtb_DelayTime = rtu_FeedBackEndDelayTime;
  }
  else
  {
    rtb_DelayTime = localDW->Memory_PreviousInput;
  }

  /* End of Switch: 'Switch2' (':5218:28') */

  /* Outputs for Atomic SubSystem: 'DelayOut' (':5218:23') */
  /* DataTypeConversion: 'Data Type Conversion' (':5218:21') */
  DelayOut_kj4v((int16_T)rtu_IsFault, rtb_DelayTime, &localB->Out,
                &localDW->DelayOut_kj4v5);

  /* End of Outputs for SubSystem: 'DelayOut' (':5218:23') */

  /* DataTypeConversion: 'Data Type Conversion1' (':5218:22') */
  rty_FeedBackBusStatus_0 = ((int32_T)localB->Out != 0);

  /* Update for UnitDelay: 'Delay Input1' (':5218:25:2')
   *
   * Block description for 'Delay Input1' (':5218:25:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE = rtu_IsFault;

  /* Update for UnitDelay: 'Delay Input1' (':5218:24:2')
   *
   * Block description for 'Delay Input1' (':5218:24:2'):
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE_evfm = rtu_IsFault;

  /* Update for Memory: 'Memory' (':5218:26') */
  localDW->Memory_PreviousInput = rtb_DelayTime;
  return rty_FeedBackBusStatus_0;
}

/* System initialize for atomic system: 'FeedBack' (':5070') */
void FeedBack_Init(void)
{
  /* SystemInitialize for Atomic SubSystem: 'PowerUpBlank' (':5103') */
  FeedBackDelay_Init(&B.PowerUpBlank, &DW.PowerUpBlank);

  /* End of SystemInitialize for SubSystem: 'PowerUpBlank' (':5103') */

  /* SystemInitialize for IfAction SubSystem: 'Subsystem10' (':5173') */
  /* SystemInitialize for Atomic SubSystem: 'FeedBackDelay4' (':5191') */
  FeedBackDelay_Init(&B.FeedBackDelay4_ls1m, &DW.FeedBackDelay4_ls1m);

  /* End of SystemInitialize for SubSystem: 'FeedBackDelay4' (':5191') */

  /* SystemInitialize for Atomic SubSystem: 'FeedBackDelay4' (':5200') */
  FeedBackDelay_Init(&B.FeedBackDelay4_eqch, &DW.FeedBackDelay4_eqch);

  /* End of SystemInitialize for SubSystem: 'FeedBackDelay4' (':5200') */

  /* SystemInitialize for Atomic SubSystem: 'FeedBackDelay4' (':5218') */
  FeedBackDelay4_Init(&B.FeedBackDelay4_cyog, &DW.FeedBackDelay4_cyog);

  /* End of SystemInitialize for SubSystem: 'FeedBackDelay4' (':5218') */

  /* SystemInitialize for Atomic SubSystem: 'FeedBackDelay4' (':5227') */
  FeedBackDelay4_Init(&B.FeedBackDelay4_j5pn, &DW.FeedBackDelay4_j5pn);

  /* End of SystemInitialize for SubSystem: 'FeedBackDelay4' (':5227') */

  /* SystemInitialize for Atomic SubSystem: 'FeedBackDelay4' (':5236') */
  FeedBackDelay4_Init(&B.FeedBackDelay4_ii3r, &DW.FeedBackDelay4_ii3r);

  /* End of SystemInitialize for SubSystem: 'FeedBackDelay4' (':5236') */

  /* SystemInitialize for Atomic SubSystem: 'FeedBackDelay4' (':5209') */
  FeedBackDelay_Init(&B.FeedBackDelay4_lhnv, &DW.FeedBackDelay4_lhnv);

  /* End of SystemInitialize for SubSystem: 'FeedBackDelay4' (':5209') */
  /* End of SystemInitialize for SubSystem: 'Subsystem10' (':5173') */

  /* SystemInitialize for Chart: 'StateKeep' (':5106') */
  DW.is_active_c2_AppMain = 0U;
  DW.is_c2_AppMain = IN_NO_ACTIVE_CHILD_jx0i;
  DW.FalseTime = MAX_uint16_T;
  DW.TrueTime = MAX_uint16_T;
  B.State = false;
}

/* Output and update for atomic system: 'FeedBack' (':5070') */
void FeedBack(void)
{
  uint32_T qY;
  uint16_T rtb_HighTime_lbgw;
  uint16_T rtb_LowTime_k5xg;
  uint8_T rtb_Switch;
  boolean_T rtb_DataTypeConversion1;
  boolean_T rtb_Merge;
  boolean_T rtb_Merge1;
  boolean_T rtb_Merge2;
  boolean_T rtb_Merge3;
  boolean_T rtb_Merge4;
  boolean_T rtb_Merge7;

  /* Outputs for Atomic SubSystem: 'PowerUpBlank' (':5103') */
  /* Constant: 'Constant1' (':5100') incorporates:
   *  Constant: 'Constant' (':5099')
   *  Constant: 'Constant2' (':5101')
   */
  rtb_DataTypeConversion1 = FeedBackDelay(35, true, 0, &B.PowerUpBlank,
    &DW.PowerUpBlank);

  /* End of Outputs for SubSystem: 'PowerUpBlank' (':5103') */

  /* If: 'If' (':5125') incorporates:
   *  Constant: 'Constant' (':5120:3')
   *  Constant: 'Constant1' (':5187')
   *  Constant: 'Constant2' (':5188')
   *  Constant: 'Constant1' (':5196')
   *  Constant: 'Constant2' (':5197')
   *  Constant: 'Constant1' (':5205')
   *  Constant: 'Constant2' (':5206')
   *  Constant: 'Constant1' (':5214')
   *  Constant: 'Constant2' (':5215')
   *  Constant: 'Constant1' (':5223')
   *  Constant: 'Constant2' (':5224')
   *  Constant: 'Constant1' (':5232')
   *  Constant: 'Constant2' (':5233')
   *  Constant: 'Constant' (':5186:3')
   *  Constant: 'Constant' (':5195:3')
   *  Constant: 'Constant' (':5204:3')
   *  Inport: 'IsMotorLock' (':5035')
   *  Inport: 'IsMotorOpenLoad' (':5036')
   *  Inport: 'IsMotorShort' (':5034')
   *  Outport: 'AmbTPreOverStatus' (':6089')
   *  RelationalOperator: 'Compare' (':5120:2')
   *  RelationalOperator: 'Compare' (':5186:2')
   *  RelationalOperator: 'Compare' (':5195:2')
   *  RelationalOperator: 'Compare' (':5204:2')
   */
  if ((int32_T)B.PwmInTargetSpeed == 0)
  {
    /* Outputs for IfAction SubSystem: 'Subsystem1' (':5162') incorporates:
     *  ActionPort: 'Action Port' (':5163')
     */
    /* SignalConversion generated from: 'FeedBackFlag' (':5165') incorporates:
     *  Constant: 'Constant' (':5164')
     */
    rtb_Merge = false;

    /* SignalConversion generated from: 'FeedBackFlag1' (':5166') incorporates:
     *  Constant: 'Constant' (':5164')
     */
    rtb_Merge1 = false;

    /* SignalConversion generated from: 'FeedBackFlag2' (':5167') incorporates:
     *  Constant: 'Constant' (':5164')
     */
    rtb_Merge2 = false;

    /* SignalConversion generated from: 'FeedBackFlag3' (':5168') incorporates:
     *  Constant: 'Constant' (':5164')
     */
    rtb_Merge3 = false;

    /* SignalConversion generated from: 'FeedBackFlag4' (':5169') incorporates:
     *  Constant: 'Constant' (':5164')
     */
    rtb_Merge4 = false;

    /* SignalConversion generated from: 'FeedBackFlag5' (':5172') incorporates:
     *  Constant: 'Constant' (':5164')
     */
    rtb_Merge7 = false;

    /* End of Outputs for SubSystem: 'Subsystem1' (':5162') */
  }
  else
  {
    /* Outputs for IfAction SubSystem: 'Subsystem10' (':5173') incorporates:
     *  ActionPort: 'Action Port' (':5182')
     */
    /* Outputs for Atomic SubSystem: 'FeedBackDelay4' (':5191') */
    rtb_Merge = FeedBackDelay(300, (int32_T)B.Status_mbnx != 0, 0,
      &B.FeedBackDelay4_ls1m, &DW.FeedBackDelay4_ls1m);

    /* End of Outputs for SubSystem: 'FeedBackDelay4' (':5191') */

    /* Outputs for Atomic SubSystem: 'FeedBackDelay4' (':5200') */
    rtb_Merge1 = FeedBackDelay(0, (int32_T)B.Status_dotr > 0, 0,
      &B.FeedBackDelay4_eqch, &DW.FeedBackDelay4_eqch);

    /* End of Outputs for SubSystem: 'FeedBackDelay4' (':5200') */

    /* Outputs for Atomic SubSystem: 'FeedBackDelay4' (':5218') */
    rtb_Merge2 = FeedBackDelay4(600, U.IsMotorShort, 0, &B.FeedBackDelay4_cyog,
      &DW.FeedBackDelay4_cyog);

    /* End of Outputs for SubSystem: 'FeedBackDelay4' (':5218') */

    /* Outputs for Atomic SubSystem: 'FeedBackDelay4' (':5227') */
    rtb_Merge3 = FeedBackDelay4(900, U.IsMotorOpenLoad, 0,
      &B.FeedBackDelay4_j5pn, &DW.FeedBackDelay4_j5pn);

    /* End of Outputs for SubSystem: 'FeedBackDelay4' (':5227') */

    /* Outputs for Atomic SubSystem: 'FeedBackDelay4' (':5236') */
    rtb_Merge4 = FeedBackDelay4(1600, U.IsMotorLock, 0, &B.FeedBackDelay4_ii3r,
      &DW.FeedBackDelay4_ii3r);

    /* End of Outputs for SubSystem: 'FeedBackDelay4' (':5236') */

    /* Outputs for Atomic SubSystem: 'FeedBackDelay4' (':5209') */
    rtb_Merge7 = FeedBackDelay(0, (int32_T)Y.AmbTPreOverStatus != 0, 0,
      &B.FeedBackDelay4_lhnv, &DW.FeedBackDelay4_lhnv);

    /* End of Outputs for SubSystem: 'FeedBackDelay4' (':5209') */
    /* End of Outputs for SubSystem: 'Subsystem10' (':5173') */
  }

  /* End of If: 'If' (':5125') */

  /* Switch: 'Switch' (':5152') incorporates:
   *  Constant: 'Constant' (':5143')
   *  Switch: 'Switch1' (':5153')
   *  Switch: 'Switch2' (':5154')
   *  Switch: 'Switch3' (':5155')
   *  Switch: 'Switch4' (':5156')
   *  Switch: 'Switch5' (':5157')
   */
  if (rtb_Merge2)
  {
    rtb_Switch = 0U;
  }
  else if (rtb_Merge3)
  {
    /* Switch: 'Switch1' (':5153') incorporates:
     *  Constant: 'Constant1' (':5144')
     */
    rtb_Switch = 1U;
  }
  else if (rtb_Merge1)
  {
    /* Switch: 'Switch2' (':5154') incorporates:
     *  Constant: 'Constant2' (':5145')
     *  Switch: 'Switch1' (':5153')
     */
    rtb_Switch = 2U;
  }
  else if (rtb_Merge4)
  {
    /* Switch: 'Switch3' (':5155') incorporates:
     *  Constant: 'Constant3' (':5146')
     *  Switch: 'Switch1' (':5153')
     *  Switch: 'Switch2' (':5154')
     */
    rtb_Switch = 3U;
  }
  else if (rtb_Merge)
  {
    /* Switch: 'Switch4' (':5156') incorporates:
     *  Constant: 'Constant4' (':5147')
     *  Switch: 'Switch1' (':5153')
     *  Switch: 'Switch2' (':5154')
     *  Switch: 'Switch3' (':5155')
     */
    rtb_Switch = 4U;
  }
  else if (rtb_Merge7)
  {
    /* Switch: 'Switch5' (':5157') incorporates:
     *  Constant: 'Constant5' (':5148')
     *  Switch: 'Switch1' (':5153')
     *  Switch: 'Switch2' (':5154')
     *  Switch: 'Switch3' (':5155')
     *  Switch: 'Switch4' (':5156')
     */
    rtb_Switch = 5U;
  }
  else
  {
    /* Switch: 'Switch4' (':5156') incorporates:
     *  Constant: 'Constant6' (':5149')
     *  Switch: 'Switch1' (':5153')
     *  Switch: 'Switch2' (':5154')
     *  Switch: 'Switch3' (':5155')
     *  Switch: 'Switch5' (':5157')
     */
    rtb_Switch = 6U;
  }

  /* End of Switch: 'Switch' (':5152') */

  /* If: 'If' (':5083') incorporates:
   *  Inport: 'BusHighTime' (':5092')
   *  Inport: 'BusLowTime' (':5091')
   *  LookupNDDirect: 'HighTime' (':5150')
   *  LookupNDDirect: 'LowTime' (':5151')
   *  Switch: 'Switch' (':5152')
   *
   * About 'HighTime' (':5150'):
   *  1-dimensional Direct Look-Up returning a Scalar,
   *
   *     Remove protection against out-of-range input in generated code: 'off'
   *
   * About 'LowTime' (':5151'):
   *  1-dimensional Direct Look-Up returning a Scalar,
   *
   *     Remove protection against out-of-range input in generated code: 'off'
   */
  if (!rtb_DataTypeConversion1)
  {
    /* Outputs for IfAction SubSystem: 'If Action Subsystem' (':5084') incorporates:
     *  ActionPort: 'Action Port' (':5085')
     */
    /* SignalConversion generated from: 'LowTime' (':5088') incorporates:
     *  Constant: 'Constant' (':5086')
     */
    rtb_LowTime_k5xg = 0U;

    /* SignalConversion generated from: 'HighTime' (':5089') incorporates:
     *  Constant: 'Constant1' (':5087')
     */
    rtb_HighTime_lbgw = 1U;

    /* End of Outputs for SubSystem: 'If Action Subsystem' (':5084') */
  }
  else
  {
    /* Outputs for IfAction SubSystem: 'If Action Subsystem1' (':5090') incorporates:
     *  ActionPort: 'Action Port' (':5093')
     */
    rtb_LowTime_k5xg = ConstP.LowTime_table[(int32_T)rtb_Switch];
    rtb_HighTime_lbgw = ConstP.HighTime_table[(int32_T)rtb_Switch];

    /* End of Outputs for SubSystem: 'If Action Subsystem1' (':5090') */
  }

  /* End of If: 'If' (':5083') */

  /* Chart: 'StateKeep' (':5106') */
  /* Gateway: FeedBack/StateKeep */
  /* During: FeedBack/StateKeep */
  if ((uint32_T)DW.is_active_c2_AppMain == 0U)
  {
    /* Entry: FeedBack/StateKeep */
    DW.is_active_c2_AppMain = 1U;

    /* Entry Internal: FeedBack/StateKeep */
    /* Transition: (':5106:5') */
    DW.TrueTime = rtb_HighTime_lbgw;
    DW.FalseTime = rtb_LowTime_k5xg;
    if ((int32_T)DW.FalseTime != 0)
    {
      /* Transition: (':5106:41') */
      DW.is_c2_AppMain = IN_FalseState;

      /* Entry 'FalseState': (':5106:6') */
      B.State = false;
    }
    else
    {
      /* Transition: (':5106:43') */
      /* Transition: (':5106:44') */
      DW.is_c2_AppMain = IN_TrueState;

      /* Entry 'TrueState': (':5106:4') */
      B.State = true;
      DW.FalseTime = 0U;
    }
  }
  else if (DW.is_c2_AppMain == IN_FalseState)
  {
    /* During 'FalseState': (':5106:6') */
    if (((int32_T)DW.FalseTime <= 1) && ((int32_T)DW.TrueTime > 0))
    {
      /* Transition: (':5106:15') */
      DW.is_c2_AppMain = IN_TrueState;

      /* Entry 'TrueState': (':5106:4') */
      B.State = true;
      DW.FalseTime = 0U;
    }
    else
    {
      B.State = false;

      /* Transition: (':5106:51') */
      if ((int32_T)DW.FalseTime > 0)
      {
        /* Transition: (':5106:28') */
        qY = (uint32_T)((uint32_T)DW.FalseTime - 1U);
        if ((uint32_T)((uint32_T)DW.FalseTime - 1U) > (uint32_T)DW.FalseTime)
        {
          qY = 0U;
        }

        DW.FalseTime = (uint16_T)qY;
      }
      else
      {
        /* Transition: (':5106:36') */
        if ((int32_T)DW.TrueTime == 0)
        {
          /* Transition: (':5106:38') */
          DW.TrueTime = rtb_HighTime_lbgw;
          DW.FalseTime = rtb_LowTime_k5xg;
        }
        else
        {
          /* Transition: (':5106:46') */
          /* Transition: (':5106:48') */
          /* Transition: (':5106:49') */
        }

        /* Transition: (':5106:39') */
      }
    }

    /* During 'TrueState': (':5106:4') */
  }
  else if (((int32_T)DW.TrueTime <= 1) && ((int32_T)rtb_LowTime_k5xg > 0))
  {
    /* Transition: (':5106:14') */
    DW.is_c2_AppMain = IN_FalseState;

    /* Entry 'FalseState': (':5106:6') */
    B.State = false;
    DW.TrueTime = rtb_HighTime_lbgw;
    DW.FalseTime = rtb_LowTime_k5xg;
  }
  else
  {
    B.State = true;

    /* Transition: (':5106:50') */
    if ((int32_T)DW.TrueTime > 0)
    {
      /* Transition: (':5106:23') */
      qY = (uint32_T)((uint32_T)DW.TrueTime - 1U);
      if ((uint32_T)((uint32_T)DW.TrueTime - 1U) > (uint32_T)DW.TrueTime)
      {
        qY = 0U;
      }

      DW.TrueTime = (uint16_T)qY;
    }
    else
    {
      /* Transition: (':5106:31') */
      /* Transition: (':5106:33') */
      /* Transition: (':5106:34') */
    }
  }

  /* End of Chart: 'StateKeep' (':5106') */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
