/*
 * File: PwmInTargetSpeedEval.c
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

#include "PwmInTargetSpeedEval.h"

/* Include model header file for global data */
#include "AppMain.h"
#include "AppMain_private.h"

/* Named constants for Chart: ''FreqValidJudge' (':6002')' */
#define IN_Invalid                     ((uint8_T)1U)
#define IN_NO_ACTIVE_CHILD_ohhc        ((uint8_T)0U)
#define IN_Valid                       ((uint8_T)2U)

/* Named constants for Chart: ''DelayOut' (':6033')' */
#define IN_Delay_dzye                  ((uint8_T)1U)
#define IN_NO_ACTIVE_CHILD_biph        ((uint8_T)0U)
#define IN_Reset_eiqc                  ((uint8_T)2U)

/* System initialize for action system: 'PwmInShort' (':6012') */
void PwmInShort_Init(void)
{
  /* InitializeConditions for UnitDelay: 'Delay Input1' (':6018:2')
   *
   * Block description for 'Delay Input1' (':6018:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_dtn5 = 0U;

  /* InitializeConditions for UnitDelay: 'Delay Input1' (':6019:2')
   *
   * Block description for 'Delay Input1' (':6019:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_ovaq = false;

  /* InitializeConditions for UnitDelay: 'Delay Input1' (':6020:2')
   *
   * Block description for 'Delay Input1' (':6020:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_egdl = false;
}

/* System reset for action system: 'PwmInShort' (':6012') */
void PwmInShort_Reset(void)
{
  /* InitializeConditions for UnitDelay: 'Delay Input1' (':6018:2')
   *
   * Block description for 'Delay Input1' (':6018:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_dtn5 = 0U;

  /* InitializeConditions for UnitDelay: 'Delay Input1' (':6019:2')
   *
   * Block description for 'Delay Input1' (':6019:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_ovaq = false;

  /* InitializeConditions for UnitDelay: 'Delay Input1' (':6020:2')
   *
   * Block description for 'Delay Input1' (':6020:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_egdl = false;
}

/* Output and update for action system: 'PwmInShort' (':6012') */
void PwmInShort(void)
{
  /* Merge: 'Merge5' (':5941') incorporates:
   *  Logic: 'Logical Operator3' (':6023')
   *  Outport: 'PwmInShortBat' (':6088')
   *  Outport: 'PwmInShortGND' (':6087')
   *  RelationalOperator: 'FixPt Relational Operator' (':6018:3')
   *  RelationalOperator: 'FixPt Relational Operator' (':6019:3')
   *  RelationalOperator: 'FixPt Relational Operator' (':6020:3')
   *  UnitDelay: 'Delay Input1' (':6018:2')
   *  UnitDelay: 'Delay Input1' (':6019:2')
   *  UnitDelay: 'Delay Input1' (':6020:2')
   *
   * Block description for 'Delay Input1' (':6018:2'):
   *
   *  Store in Global RAM
   *
   * Block description for 'Delay Input1' (':6019:2'):
   *
   *  Store in Global RAM
   *
   * Block description for 'Delay Input1' (':6020:2'):
   *
   *  Store in Global RAM
   */
  B.Merge5 = (((int32_T)Y.PwmInShortGND > (int32_T)DW.DelayInput1_DSTATE_ovaq) ||
              ((int32_T)Y.PwmInShortBat > (int32_T)DW.DelayInput1_DSTATE_egdl) ||
              (B.Duty != DW.DelayInput1_DSTATE_dtn5));

  /* Switch: 'Switch' (':6025') incorporates:
   *  Outport: 'PwmInShortBat' (':6088')
   */
  if (Y.PwmInShortBat)
  {
    /* Merge: 'Merge7' (':5943') incorporates:
     *  Constant: 'Duty0' (':6021')
     */
    B.Merge7 = 0U;
  }
  else
  {
    /* Merge: 'Merge7' (':5943') incorporates:
     *  Constant: 'Duty100' (':6022')
     */
    B.Merge7 = 1000U;
  }

  /* End of Switch: 'Switch' (':6025') */

  /* Merge: 'Merge' (':5937') incorporates:
   *  Constant: 'Constant' (':6017')
   *  SignalConversion generated from: 'PwmInErr' (':6028')
   */
  B.Merge = true;

  /* Update for UnitDelay: 'Delay Input1' (':6018:2')
   *
   * Block description for 'Delay Input1' (':6018:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_dtn5 = B.Duty;

  /* Update for UnitDelay: 'Delay Input1' (':6019:2') incorporates:
   *  Outport: 'PwmInShortGND' (':6087')
   *
   * Block description for 'Delay Input1' (':6019:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_ovaq = Y.PwmInShortGND;

  /* Update for UnitDelay: 'Delay Input1' (':6020:2') incorporates:
   *  Outport: 'PwmInShortBat' (':6088')
   *
   * Block description for 'Delay Input1' (':6020:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_egdl = Y.PwmInShortBat;
}

/* Output and update for atomic system: 'DutyBufUpdate' (':5966') */
void DutyBufUpdate(uint16_T rtu_data, uint16_T rty_buf[6])
{
  int32_T i;

  /*  # eml */
  /* MATLAB Function 'PwmInTargetSpeedEval/NormalState/PwmInSpeedCtrl/PwmInNotShort/DutyFilter/Enabled Subsystem/DutyFilter/DutyBufUpdate': (':5966:1') */
  /* (':5966:1:3') */
  /* (':5966:1:4') */
  for (i = 0; i < 5; i++)
  {
    /* (':5966:1:4') */
    /* (':5966:1:5') */
    rty_buf[(int32_T)(5 - i)] = rty_buf[(int32_T)(4 - i)];
  }

  /* (':5966:1:8') */
  rty_buf[0] = rtu_data;
}

/* Output and update for atomic system: 'BufDataSameFirstCnt' (':5976') */
void BufDataSameFirstCnt(const uint16_T rtu_buf[6], uint16_T rtu_tolerance,
  uint16_T *rty_samecnt)
{
  int32_T i;
  int32_T tmp;
  uint32_T qY;
  uint16_T b_max;
  uint16_T b_min;

  /* MATLAB Function 'PwmInTargetSpeedEval/NormalState/PwmInSpeedCtrl/PwmInNotShort/DutyFilter/Enabled Subsystem/DutyFilter/DutyValid/BufDataSameFirstCnt': (':5976:1') */
  if (rtu_buf[0] >= rtu_tolerance)
  {
    /* (':5976:1:2') */
    /* (':5976:1:3') */
    qY = (uint32_T)((uint32_T)rtu_buf[0] - (uint32_T)rtu_tolerance);
    if (qY > (uint32_T)rtu_buf[0])
    {
      qY = 0U;
    }

    b_min = (uint16_T)qY;
  }
  else
  {
    /* (':5976:1:5') */
    b_min = 0U;
  }

  /* (':5976:1:8') */
  qY = (uint32_T)((uint32_T)rtu_buf[0] + (uint32_T)rtu_tolerance);
  if (qY > 65535U)
  {
    qY = 65535U;
  }

  b_max = (uint16_T)qY;
  if ((int32_T)(uint16_T)qY > 1000)
  {
    /* (':5976:1:9') */
    /* (':5976:1:10') */
    b_max = 1000U;
  }

  /* (':5976:1:13') */
  *rty_samecnt = 1U;

  /* (':5976:1:14') */
  for (i = 0; i < 5; i++)
  {
    /* (':5976:1:14') */
    tmp = (int32_T)rtu_buf[(int32_T)(i + 1)];
    if ((tmp >= (int32_T)b_min) && (tmp <= (int32_T)b_max))
    {
      /* (':5976:1:15') */
      /* (':5976:1:16') */
      qY = (uint32_T)((uint32_T)*rty_samecnt + 1U);
      if ((uint32_T)((uint32_T)*rty_samecnt + 1U) > 65535U)
      {
        qY = 65535U;
      }

      *rty_samecnt = (uint16_T)qY;
    }
  }
}

/* System initialize for enable system: 'DutyFilter' (':5951') */
void DutyFilter_Init(B_DutyFilter_T *localB, DW_DutyFilter_T *localDW)
{
  int32_T i;

  /* InitializeConditions for Memory: 'DutyBuf' (':5956') */
  for (i = 0; i < 6; i++)
  {
    localDW->DutyBuf_PreviousInput[i] = 2000U;
  }

  /* End of InitializeConditions for Memory: 'DutyBuf' (':5956') */

  /* SystemInitialize for Enabled SubSystem: 'Enabled Subsystem' (':5957') */
  /* SystemInitialize for Merge: 'Merge1' (':5986') */
  localB->Merge1 = 0U;

  /* End of SystemInitialize for SubSystem: 'Enabled Subsystem' (':5957') */
}

/* System reset for enable system: 'DutyFilter' (':5951') */
void DutyFilter_Reset(DW_DutyFilter_T *localDW)
{
  int32_T i;

  /* InitializeConditions for Memory: 'DutyBuf' (':5956') */
  for (i = 0; i < 6; i++)
  {
    localDW->DutyBuf_PreviousInput[i] = 2000U;
  }

  /* End of InitializeConditions for Memory: 'DutyBuf' (':5956') */
}

/* Disable for enable system: 'DutyFilter' (':5951') */
void DutyFilter_Disable(DW_DutyFilter_T *localDW)
{
  localDW->DutyFilter_MODE = false;
}

/* Output and update for enable system: 'DutyFilter' (':5951') */
void DutyFilter(boolean_T rtu_Enable, uint16_T rtu_Duty, uint16_T rtu_DutyLast,
                boolean_T rtu_IsPwmDutyUpdated, uint16_T *rty_FilteredDuty,
                boolean_T *rty_Valid, B_DutyFilter_T *localB, DW_DutyFilter_T
                *localDW)
{
  int32_T i;
  uint16_T rtb_samecnt;
  boolean_T rtb_RelationalOperator1[6];
  boolean_T tmp;

  /* Outputs for Enabled SubSystem: 'DutyFilter' (':5951') incorporates:
   *  EnablePort: 'Enable' (':5955')
   */
  if (rtu_Enable)
  {
    if (!localDW->DutyFilter_MODE)
    {
      DutyFilter_Reset(localDW);
      localDW->DutyFilter_MODE = true;
    }

    /* Outputs for Enabled SubSystem: 'Enabled Subsystem' (':5957') incorporates:
     *  EnablePort: 'Enable' (':5961')
     */
    if (rtu_IsPwmDutyUpdated)
    {
      /* MATLAB Function: 'DutyBufUpdate' (':5966') incorporates:
       *  Memory: 'DutyBuf' (':5956')
       */
      for (i = 0; i < 6; i++)
      {
        localB->buf[i] = localDW->DutyBuf_PreviousInput[i];
      }

      DutyBufUpdate(rtu_Duty, localB->buf);

      /* End of MATLAB Function: 'DutyBufUpdate' (':5966') */

      /* RelationalOperator: 'Relational Operator1' (':5987') incorporates:
       *  Constant: 'DutyMax' (':5971')
       */
      for (i = 0; i < 6; i++)
      {
        rtb_RelationalOperator1[i] = ((int32_T)localB->buf[i] <= 1000);
      }

      /* End of RelationalOperator: 'Relational Operator1' (':5987') */

      /* Logic: 'Logical Operator' (':5985') */
      tmp = rtb_RelationalOperator1[0];
      for (i = 0; i < 5; i++)
      {
        tmp = (tmp && rtb_RelationalOperator1[(int32_T)(i + 1)]);
      }

      *rty_Valid = tmp;

      /* End of Logic: 'Logical Operator' (':5985') */

      /* If: 'If' (':5984') */
      if (!*rty_Valid)
      {
        /* Outputs for IfAction SubSystem: 'DutyInvalid' (':5967') incorporates:
         *  ActionPort: 'Action Port' (':5969')
         */
        /* Merge: 'Merge1' (':5986') incorporates:
         *  Inport: 'DutyLast' (':5968')
         */
        localB->Merge1 = rtu_DutyLast;

        /* End of Outputs for SubSystem: 'DutyInvalid' (':5967') */
      }
      else
      {
        /* Outputs for IfAction SubSystem: 'DutyValid' (':5972') incorporates:
         *  ActionPort: 'Action Port' (':5975')
         */
        /* MATLAB Function: 'BufDataSameFirstCnt' (':5976') incorporates:
         *  Constant: 'DutyTolerance' (':5978')
         */
        BufDataSameFirstCnt(localB->buf, 20, &rtb_samecnt);

        /* Switch: 'Switch' (':5982') incorporates:
         *  Constant: 'DutySameCntTHR' (':5977')
         *  RelationalOperator: 'Relational Operator' (':5979')
         */
        if ((int32_T)rtb_samecnt >= 4)
        {
          /* Merge: 'Merge1' (':5986') incorporates:
           *  SignalConversion generated from: 'FilteredDuty' (':5983')
           */
          localB->Merge1 = localB->buf[0];
        }
        else
        {
          /* Merge: 'Merge1' (':5986') incorporates:
           *  SignalConversion generated from: 'FilteredDuty' (':5983')
           */
          localB->Merge1 = rtu_DutyLast;
        }

        /* End of Switch: 'Switch' (':5982') */
        /* End of Outputs for SubSystem: 'DutyValid' (':5972') */
      }

      /* End of If: 'If' (':5984') */
    }

    /* End of Outputs for SubSystem: 'Enabled Subsystem' (':5957') */

    /* Switch: 'Switch' (':6535') */
    if (rtu_IsPwmDutyUpdated)
    {
      *rty_FilteredDuty = localB->Merge1;
    }
    else
    {
      *rty_FilteredDuty = rtu_DutyLast;
    }

    /* End of Switch: 'Switch' (':6535') */

    /* Update for Memory: 'DutyBuf' (':5956') */
    for (i = 0; i < 6; i++)
    {
      localDW->DutyBuf_PreviousInput[i] = localB->buf[i];
    }

    /* End of Update for Memory: 'DutyBuf' (':5956') */
  }
  else if (localDW->DutyFilter_MODE)
  {
    DutyFilter_Disable(localDW);
  }

  /* End of Outputs for SubSystem: 'DutyFilter' (':5951') */
}

/* System initialize for atomic system: 'FreqValidJudge' (':5997') */
void FreqValidJudge_Init(boolean_T *rty_Valid, DW_FreqValidJudge_T *localDW)
{
  /* SystemInitialize for Chart: 'FreqValidJudge' (':6002') */
  localDW->is_active_c10_AppMain = 0U;
  localDW->is_c10_AppMain = IN_NO_ACTIVE_CHILD_ohhc;
  localDW->ValidCnt = 0U;
  localDW->InvalidCnt = 0U;
  *rty_Valid = false;
}

/* System reset for atomic system: 'FreqValidJudge' (':5997') */
void FreqValidJudge_Reset(boolean_T *rty_Valid, DW_FreqValidJudge_T *localDW)
{
  /* SystemReset for Chart: 'FreqValidJudge' (':6002') */
  localDW->is_active_c10_AppMain = 0U;
  localDW->is_c10_AppMain = IN_NO_ACTIVE_CHILD_ohhc;
  localDW->ValidCnt = 0U;
  localDW->InvalidCnt = 0U;
  *rty_Valid = false;
}

/* Output and update for atomic system: 'FreqValidJudge' (':5997') */
boolean_T FreqValidJudge(uint16_T rtu_Freq, DW_FreqValidJudge_T *localDW)
{
  boolean_T rty_Valid_0;
  uint32_T tmp;

  /* Chart: 'FreqValidJudge' (':6002') incorporates:
   *  Constant: 'FreqMax' (':5999')
   *  Constant: 'FreqMin' (':6000')
   *  Constant: 'FreqTolerance' (':6001')
   *  Constant: 'FreqValidJudgeFilterTime' (':6003')
   */
  /* Gateway: PwmInTargetSpeedEval/NormalState/PwmInSpeedCtrl/PwmInNotShort/FreqValidJudge/FreqValidJudge */
  /* During: PwmInTargetSpeedEval/NormalState/PwmInSpeedCtrl/PwmInNotShort/FreqValidJudge/FreqValidJudge */
  if ((uint32_T)localDW->is_active_c10_AppMain == 0U)
  {
    /* Entry: PwmInTargetSpeedEval/NormalState/PwmInSpeedCtrl/PwmInNotShort/FreqValidJudge/FreqValidJudge */
    localDW->is_active_c10_AppMain = 1U;

    /* Entry Internal: PwmInTargetSpeedEval/NormalState/PwmInSpeedCtrl/PwmInNotShort/FreqValidJudge/FreqValidJudge */
    /* Transition: (':6002:7') */
    tmp = (uint32_T)((uint32_T)rtu_Freq + 3U);
    if ((uint32_T)((uint32_T)rtu_Freq + 3U) > 65535U)
    {
      tmp = 65535U;
    }

    if (((int32_T)rtu_Freq <= 3093) && ((int32_T)tmp >= 970))
    {
      /* Transition: (':6002:55') */
      localDW->is_c10_AppMain = IN_Valid;

      /* Entry 'Valid': (':6002:8') */
      rty_Valid_0 = true;
      localDW->InvalidCnt = 1U;
    }
    else
    {
      /* Transition: (':6002:57') */
      /* Transition: (':6002:58') */
      localDW->is_c10_AppMain = IN_Invalid;

      /* Entry 'Invalid': (':6002:36') */
      rty_Valid_0 = false;
      localDW->ValidCnt = 1U;
    }
  }
  else if (localDW->is_c10_AppMain == IN_Invalid)
  {
    /* During 'Invalid': (':6002:36') */
    if ((int32_T)localDW->ValidCnt >= 20)
    {
      /* Transition: (':6002:29') */
      localDW->is_c10_AppMain = IN_Valid;

      /* Entry 'Valid': (':6002:8') */
      rty_Valid_0 = true;
      localDW->InvalidCnt = 1U;
    }
    else
    {
      /* Transition: (':6002:42') */
      rty_Valid_0 = false;
      if (((int32_T)rtu_Freq <= 3090) && ((int32_T)rtu_Freq >= 970))
      {
        /* Transition: (':6002:43') */
        tmp = (uint32_T)((uint32_T)localDW->ValidCnt + 1U);
        if ((uint32_T)((uint32_T)localDW->ValidCnt + 1U) > 65535U)
        {
          tmp = 65535U;
        }

        localDW->ValidCnt = (uint16_T)tmp;
      }
      else
      {
        /* Transition: (':6002:45') */
        localDW->ValidCnt = 1U;
      }
    }

    /* During 'Valid': (':6002:8') */
  }
  else if ((int32_T)localDW->InvalidCnt >= 20)
  {
    /* Transition: (':6002:28') */
    localDW->is_c10_AppMain = IN_Invalid;

    /* Entry 'Invalid': (':6002:36') */
    rty_Valid_0 = false;
    localDW->ValidCnt = 1U;
  }
  else
  {
    /* Transition: (':6002:10') */
    rty_Valid_0 = true;
    tmp = (uint32_T)((uint32_T)rtu_Freq + 3U);
    if ((uint32_T)((uint32_T)rtu_Freq + 3U) > 65535U)
    {
      tmp = 65535U;
    }

    if (((int32_T)rtu_Freq > 3093) || ((int32_T)tmp < 970))
    {
      /* Transition: (':6002:12') */
      tmp = (uint32_T)((uint32_T)localDW->InvalidCnt + 1U);
      if ((uint32_T)((uint32_T)localDW->InvalidCnt + 1U) > 65535U)
      {
        tmp = 65535U;
      }

      localDW->InvalidCnt = (uint16_T)tmp;
    }
    else
    {
      /* Transition: (':6002:33') */
      localDW->InvalidCnt = 1U;
    }
  }

  /* End of Chart: 'FreqValidJudge' (':6002') */
  return rty_Valid_0;
}

/* System initialize for action system: 'PwmInNotShort' (':5944') */
void PwmInNotShort_Init(void)
{
  boolean_T rtb_Valid;

  /* InitializeConditions for UnitDelay: 'Delay Input1' (':5950:2')
   *
   * Block description for 'Delay Input1' (':5950:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_evnj = false;

  /* SystemInitialize for Atomic SubSystem: 'FreqValidJudge' (':5997') */
  FreqValidJudge_Init(&rtb_Valid, &DW.FreqValidJudge_h5v4);

  /* End of SystemInitialize for SubSystem: 'FreqValidJudge' (':5997') */

  /* SystemInitialize for Enabled SubSystem: 'DutyFilter' (':5951') */
  DutyFilter_Init(&B.DutyFilter_aldi, &DW.DutyFilter_aldi);

  /* End of SystemInitialize for SubSystem: 'DutyFilter' (':5951') */
}

/* System reset for action system: 'PwmInNotShort' (':5944') */
void PwmInNotShort_Reset(void)
{
  boolean_T rtb_Valid;

  /* InitializeConditions for UnitDelay: 'Delay Input1' (':5950:2')
   *
   * Block description for 'Delay Input1' (':5950:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_evnj = false;

  /* SystemReset for Atomic SubSystem: 'FreqValidJudge' (':5997') */
  FreqValidJudge_Reset(&rtb_Valid, &DW.FreqValidJudge_h5v4);

  /* End of SystemReset for SubSystem: 'FreqValidJudge' (':5997') */
}

/* Disable for action system: 'PwmInNotShort' (':5944') */
void PwmInNotShort_Disable(void)
{
  /* Disable for Enabled SubSystem: 'DutyFilter' (':5951') */
  if (DW.DutyFilter_aldi.DutyFilter_MODE)
  {
    DutyFilter_Disable(&DW.DutyFilter_aldi);
  }

  /* End of Disable for SubSystem: 'DutyFilter' (':5951') */
}

/* Output and update for action system: 'PwmInNotShort' (':5944') */
void PwmInNotShort(void)
{
  boolean_T rtb_Valid;

  /* Outputs for Atomic SubSystem: 'FreqValidJudge' (':5997') */
  rtb_Valid = FreqValidJudge(B.Freq, &DW.FreqValidJudge_h5v4);

  /* End of Outputs for SubSystem: 'FreqValidJudge' (':5997') */

  /* Outputs for Enabled SubSystem: 'DutyFilter' (':5951') */
  /* SignalConversion generated from: 'Enable' (':5955') */
  DutyFilter(rtb_Valid, B.HighDuty, B.DutyLast, B.Compare, &B.Switch,
             &B.DutyValid, &B.DutyFilter_aldi, &DW.DutyFilter_aldi);

  /* End of Outputs for SubSystem: 'DutyFilter' (':5951') */

  /* Logic: 'Logical Operator' (':6005') incorporates:
   *  RelationalOperator: 'Relational Operator' (':6007')
   *  RelationalOperator: 'FixPt Relational Operator' (':5950:3')
   *  UnitDelay: 'Delay Input1' (':5950:2')
   *
   * Block description for 'Delay Input1' (':5950:2'):
   *
   *  Store in Global RAM
   */
  B.Merge5 = (((int32_T)B.DutyValid > (int32_T)DW.DelayInput1_DSTATE_evnj) ||
              (B.Switch != B.DutyLast));

  /* Switch: 'Switch' (':6008') */
  if (rtb_Valid)
  {
    B.Merge7 = B.Switch;
  }
  else
  {
    B.Merge7 = B.DutyLast;
  }

  /* End of Switch: 'Switch' (':6008') */

  /* Logic: 'Logical Operator2' (':6006') */
  B.Merge = !rtb_Valid;

  /* Update for UnitDelay: 'Delay Input1' (':5950:2')
   *
   * Block description for 'Delay Input1' (':5950:2'):
   *
   *  Store in Global RAM
   */
  DW.DelayInput1_DSTATE_evnj = B.DutyValid;
}

/* System initialize for atomic system: 'SpeedDelayOut' (':6029') */
void SpeedDelayOut_Init(int16_T *rty_Out, DW_SpeedDelayOut_T *localDW)
{
  /* SystemInitialize for Chart: 'DelayOut' (':6033') */
  localDW->is_active_c12_AppMain = 0U;
  localDW->is_c12_AppMain = IN_NO_ACTIVE_CHILD_biph;
  localDW->DelayCnt = 0U;
  *rty_Out = 0;
}

/* Output and update for atomic system: 'SpeedDelayOut' (':6029') */
void SpeedDelayOut(int16_T rtu_In, uint16_T rtu_DelayTime, boolean_T rtu_Rst,
                   int16_T *rty_Out, DW_SpeedDelayOut_T *localDW)
{
  uint32_T tmp;

  /* Chart: 'DelayOut' (':6033') incorporates:
   *  Memory: 'OutLast' (':6034')
   */
  /* Gateway: PwmInTargetSpeedEval/NormalState/PwmInSpeedCtrl/SpeedDelayOut/DelayOut */
  /* During: PwmInTargetSpeedEval/NormalState/PwmInSpeedCtrl/SpeedDelayOut/DelayOut */
  if ((uint32_T)localDW->is_active_c12_AppMain == 0U)
  {
    /* Entry: PwmInTargetSpeedEval/NormalState/PwmInSpeedCtrl/SpeedDelayOut/DelayOut */
    localDW->is_active_c12_AppMain = 1U;

    /* Entry Internal: PwmInTargetSpeedEval/NormalState/PwmInSpeedCtrl/SpeedDelayOut/DelayOut */
    /* Transition: (':6033:27') */
    localDW->is_c12_AppMain = IN_Reset_eiqc;

    /* Entry 'Reset': (':6033:38') */
    localDW->DelayCnt = 2U;
    *rty_Out = localDW->OutLast_PreviousInput;
  }
  else if (localDW->is_c12_AppMain == IN_Delay_dzye)
  {
    /* During 'Delay': (':6033:28') */
    if (rtu_Rst)
    {
      /* Transition: (':6033:40') */
      localDW->is_c12_AppMain = IN_Reset_eiqc;

      /* Entry 'Reset': (':6033:38') */
      localDW->DelayCnt = 2U;
      *rty_Out = localDW->OutLast_PreviousInput;

      /* Transition: (':6033:33') */
    }
    else if (localDW->DelayCnt < rtu_DelayTime)
    {
      /* Transition: (':6033:34') */
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
      /* Transition: (':6033:35') */
      /* Transition: (':6033:37') */
      *rty_Out = rtu_In;

      /* Transition: (':6033:36') */
    }

    /* During 'Reset': (':6033:38') */
  }
  else if (!rtu_Rst)
  {
    /* Transition: (':6033:39') */
    localDW->is_c12_AppMain = IN_Delay_dzye;
  }

  /* End of Chart: 'DelayOut' (':6033') */

  /* Update for Memory: 'OutLast' (':6034') */
  localDW->OutLast_PreviousInput = *rty_Out;
}

/* System initialize for atomic system: 'PwmInTargetSpeedEval' (':5612') */
void PwmInTargetSpeedEval_Init(void)
{
  /* Start for If: 'If' (':5617') */
  DW.If_ActiveSubsystem = -1;

  /* SystemInitialize for IfAction SubSystem: 'NormalState' (':5621') */
  /* SystemInitialize for Atomic SubSystem: 'PwmInSpeedCtrl' (':5628') */
  /* Start for If: 'If' (':5921') */
  DW.If_ActiveSubsystem_dy0c = -1;

  /* Start for If: 'If1' (':5922') */
  DW.If1_ActiveSubsystem = -1;

  /* InitializeConditions for Memory: 'DutyLast' (':5637') */
  DW.DutyLast_PreviousInput = 80U;

  /* InitializeConditions for UnitDelay: 'Unit Delay' (':6036') */
  DW.UnitDelay_DSTATE_bkn4 = 1U;

  /* SystemInitialize for IfAction SubSystem: 'PwmInShort' (':6012') */
  PwmInShort_Init();

  /* End of SystemInitialize for SubSystem: 'PwmInShort' (':6012') */

  /* SystemInitialize for IfAction SubSystem: 'PwmInNotShort' (':5944') */
  PwmInNotShort_Init();

  /* End of SystemInitialize for SubSystem: 'PwmInNotShort' (':5944') */

  /* SystemInitialize for IfAction SubSystem: 'ErrSpeedCtrl' (':5802') */
  /* InitializeConditions for Memory: 'Memory' (':5831') */
  DW.Memory_PreviousInput = 0U;

  /* End of SystemInitialize for SubSystem: 'ErrSpeedCtrl' (':5802') */

  /* SystemInitialize for Atomic SubSystem: 'SpeedDelayOut' (':6029') */
  SpeedDelayOut_Init(&B.Out, &DW.SpeedDelayOut_p0zg);

  /* End of SystemInitialize for SubSystem: 'SpeedDelayOut' (':6029') */
  /* End of SystemInitialize for SubSystem: 'PwmInSpeedCtrl' (':5628') */
  /* End of SystemInitialize for SubSystem: 'NormalState' (':5621') */
}

/* Output and update for atomic system: 'PwmInTargetSpeedEval' (':5612') */
void PwmInTargetSpeedEval(void)
{
  int32_T u0;
  int32_T u1;
  int16_T rtb_Speed;
  uint16_T Merge3;
  uint16_T rtb_Delay;
  uint16_T rtb_Sector;
  uint16_T y;
  int8_T rtAction;
  int8_T rtPrevAction;
  boolean_T rtb_Rst;

  /* If: 'If' (':5617') incorporates:
   *  Constant: 'InitSpeed' (':6048')
   *  Constant: 'PowerOnBlankTimeThreshold' (':6053')
   *  Memory: 'PowerOnBlankTimeCnt' (':6052')
   *  RelationalOperator: 'Relational Operator' (':6054')
   */
  rtPrevAction = DW.If_ActiveSubsystem;
  rtAction = (int8_T)((int32_T)DW.PowerOnBlankTimeCnt_PreviousInp >= 20);
  DW.If_ActiveSubsystem = rtAction;
  if (((int32_T)rtPrevAction != (int32_T)rtAction) && ((int32_T)rtPrevAction ==
       1))
  {
    /* Disable for Atomic SubSystem: 'PwmInSpeedCtrl' (':5628') */
    /* Disable for If: 'If' (':5921') */
    if ((int32_T)DW.If_ActiveSubsystem_dy0c == 1)
    {
      PwmInNotShort_Disable();
    }

    DW.If_ActiveSubsystem_dy0c = -1;

    /* End of Disable for If: 'If' (':5921') */

    /* Disable for If: 'If1' (':5922') */
    DW.If1_ActiveSubsystem = -1;

    /* End of Disable for SubSystem: 'PwmInSpeedCtrl' (':5628') */
  }

  if ((int32_T)rtAction == 0)
  {
    /* Outputs for IfAction SubSystem: 'PowerOnBlankState' (':6042') incorporates:
     *  ActionPort: 'Action Port' (':6044')
     */
    /* Update for Memory: 'PowerOnBlankTimeCnt' (':6052') incorporates:
     *  Constant: 'AddOne' (':6046')
     *  Sum: 'Add' (':6045')
     */
    DW.PowerOnBlankTimeCnt_PreviousInp = (uint16_T)(uint32_T)((uint32_T)
      DW.PowerOnBlankTimeCnt_PreviousInp + 1U);
    B.PwmInTargetSpeed = 0;

    /* Merge: 'Merge2' (':5620') incorporates:
     *  Constant: 'InitErrSts' (':6047')
     *  Constant: 'InitSpeed' (':6048')
     */
    B.PwmInError = false;

    /* End of Outputs for SubSystem: 'PowerOnBlankState' (':6042') */
  }
  else
  {
    /* Outputs for IfAction SubSystem: 'NormalState' (':5621') incorporates:
     *  ActionPort: 'Action Port' (':5627')
     */
    /* Outputs for Atomic SubSystem: 'PwmInSpeedCtrl' (':5628') */
    /* Memory: 'DutyLast' (':5637') */
    B.DutyLast = DW.DutyLast_PreviousInput;

    /* If: 'If' (':5921') incorporates:
     *  Logic: 'Logical Operator1' (':5936')
     *  Outport: 'PwmInShortBat' (':6088')
     *  Outport: 'PwmInShortGND' (':6087')
     */
    rtPrevAction = DW.If_ActiveSubsystem_dy0c;
    rtAction = (int8_T)((!Y.PwmInShortGND) && (!Y.PwmInShortBat));
    DW.If_ActiveSubsystem_dy0c = rtAction;
    if (((int32_T)rtPrevAction != (int32_T)rtAction) && ((int32_T)rtPrevAction ==
         1))
    {
      PwmInNotShort_Disable();
    }

    if ((int32_T)rtAction == 0)
    {
      if (0 != (int32_T)rtPrevAction)
      {
        /* SystemReset for IfAction SubSystem: 'PwmInShort' (':6012') incorporates:
         *  ActionPort: 'Action Port' (':6015')
         */
        /* SystemReset for If: 'If' (':5921') */
        PwmInShort_Reset();

        /* End of SystemReset for SubSystem: 'PwmInShort' (':6012') */
      }

      /* Outputs for IfAction SubSystem: 'PwmInShort' (':6012') incorporates:
       *  ActionPort: 'Action Port' (':6015')
       */
      PwmInShort();

      /* End of Outputs for SubSystem: 'PwmInShort' (':6012') */
    }
    else
    {
      if (1 != (int32_T)rtPrevAction)
      {
        /* SystemReset for IfAction SubSystem: 'PwmInNotShort' (':5944') incorporates:
         *  ActionPort: 'Action Port' (':5948')
         */
        /* SystemReset for If: 'If' (':5921') */
        PwmInNotShort_Reset();

        /* End of SystemReset for SubSystem: 'PwmInNotShort' (':5944') */
      }

      /* Outputs for IfAction SubSystem: 'PwmInNotShort' (':5944') incorporates:
       *  ActionPort: 'Action Port' (':5948')
       */
      PwmInNotShort();

      /* End of Outputs for SubSystem: 'PwmInNotShort' (':5944') */
    }

    /* End of If: 'If' (':5921') */

    /* If: 'If1' (':5922') incorporates:
     *  Inport: 'FilteredVoltage' (':5038')
     *  Inport: 'LastDelay' (':5925')
     *  Inport: 'LastSpeed' (':5924')
     *  Inport: 'SpeedCurveSectorLast' (':5926')
     *  Logic: 'Logical Operator' (':5935')
     *  Memory: 'LastDelay' (':5933')
     *  Memory: 'LastSpeed' (':5934')
     *  RelationalOperator: 'FixPt Relational Operator' (':5636:3')
     *  UnitDelay: 'Unit Delay' (':6036')
     *  UnitDelay: 'Delay Input1' (':5636:2')
     *
     * Block description for 'Delay Input1' (':5636:2'):
     *
     *  Store in Global RAM
     */
    rtPrevAction = DW.If1_ActiveSubsystem;
    if (B.Merge)
    {
      rtAction = 0;
    }
    else if (B.Merge5 || ((int32_T)U.FilteredVoltage != (int32_T)
                          DW.DelayInput1_DSTATE))
    {
      rtAction = 1;
    }
    else
    {
      rtAction = 2;
    }

    DW.If1_ActiveSubsystem = rtAction;
    switch (rtAction)
    {
     case 0:
      if ((int32_T)rtAction != (int32_T)rtPrevAction)
      {
        /* InitializeConditions for IfAction SubSystem: 'ErrSpeedCtrl' (':5802') incorporates:
         *  ActionPort: 'Action Port' (':5808')
         */
        /* InitializeConditions for If: 'If1' (':5922') incorporates:
         *  Memory: 'Memory' (':5831')
         */
        DW.Memory_PreviousInput = 0U;

        /* End of InitializeConditions for SubSystem: 'ErrSpeedCtrl' (':5802') */
      }

      /* Outputs for IfAction SubSystem: 'ErrSpeedCtrl' (':5802') incorporates:
       *  ActionPort: 'Action Port' (':5808')
       */
      /* Switch: 'Switch4' (':5836') incorporates:
       *  Constant: 'Constant' (':5812')
       *  Outport: 'PwmInShortGND' (':6087')
       *  UnitDelay: 'Unit Delay' (':6036')
       */
      if (Y.PwmInShortGND)
      {
        rtb_Sector = 0U;
      }
      else
      {
        rtb_Sector = DW.UnitDelay_DSTATE_bkn4;
      }

      /* End of Switch: 'Switch4' (':5836') */

      /* Switch: 'Switch2' (':5834') incorporates:
       *  Constant: 'Constant1' (':5813')
       *  Memory: 'LastDelay' (':5933')
       *  Outport: 'PwmInShortBat' (':6088')
       *  Outport: 'PwmInShortGND' (':6087')
       *  Switch: 'Switch3' (':5835')
       */
      if (Y.PwmInShortGND)
      {
        rtb_Delay = 380U;
      }
      else if (Y.PwmInShortBat)
      {
        /* Switch: 'Switch3' (':5835') incorporates:
         *  Constant: 'Constant6' (':5817')
         */
        rtb_Delay = 180U;
      }
      else
      {
        rtb_Delay = DW.LastDelay_PreviousInput;
      }

      /* End of Switch: 'Switch2' (':5834') */

      /* Switch: 'Switch' (':5832') incorporates:
       *  Constant: 'Constant3' (':5814')
       *  Memory: 'LastSpeed' (':5934')
       *  Outport: 'PwmInShortBat' (':6088')
       *  Outport: 'PwmInShortGND' (':6087')
       *  Switch: 'Switch1' (':5833')
       */
      if (Y.PwmInShortGND)
      {
        rtb_Speed = 0;
      }
      else if (Y.PwmInShortBat)
      {
        /* Switch: 'Switch1' (':5833') incorporates:
         *  Constant: 'Constant4' (':5815')
         */
        rtb_Speed = 0;
      }
      else
      {
        rtb_Speed = DW.LastSpeed_PreviousInput;
      }

      /* End of Switch: 'Switch' (':5832') */

      /* RelationalOperator: 'Compare' (':5811:2') incorporates:
       *  Constant: 'Constant' (':5811:3')
       *  Memory: 'Memory' (':5831')
       */
      rtb_Rst = ((int32_T)DW.Memory_PreviousInput == 0);

      /* Update for Memory: 'Memory' (':5831') incorporates:
       *  Bias: 'Bias' (':5809')
       */
      DW.Memory_PreviousInput = (uint16_T)(uint32_T)((uint32_T)
        DW.Memory_PreviousInput + 1U);

      /* End of Outputs for SubSystem: 'ErrSpeedCtrl' (':5802') */
      break;

     case 1:
      /* Outputs for IfAction SubSystem: 'DutySpeedCtrl' (':5638') incorporates:
       *  ActionPort: 'Action Port' (':5642')
       */
      /* Outputs for Atomic SubSystem: 'SpeedCurve' (':5655') */
      /* SwitchCase: 'Switch Case' (':5712') incorporates:
       *  UnitDelay: 'Unit Delay' (':6036')
       */
      switch ((int32_T)DW.UnitDelay_DSTATE_bkn4)
      {
       case 0:
        /* Outputs for IfAction SubSystem: 'Sector0' (':5685') incorporates:
         *  ActionPort: 'Action Port' (':5687')
         */
        /* Switch: 'Switch' (':5693') incorporates:
         *  Constant: 'Constant' (':5688:3')
         *  RelationalOperator: 'Compare' (':5688:2')
         */
        if ((int32_T)B.Merge7 > 950)
        {
          /* Merge: 'Merge3' (':5684') incorporates:
           *  Constant: 'Constant' (':5690')
           */
          Merge3 = 0U;
        }
        else
        {
          /* Merge: 'Merge3' (':5684') incorporates:
           *  Constant: 'Constant' (':5689:3')
           *  RelationalOperator: 'Compare' (':5689:2')
           */
          Merge3 = (uint16_T)((int32_T)B.Merge7 >= 100);
        }

        /* End of Switch: 'Switch' (':5693') */
        /* End of Outputs for SubSystem: 'Sector0' (':5685') */
        break;

       case 1:
        /* Outputs for IfAction SubSystem: 'Sector1' (':5696') incorporates:
         *  ActionPort: 'Action Port' (':5698')
         */
        /* Switch: 'Switch' (':5703') incorporates:
         *  Constant: 'Constant' (':5699:3')
         *  RelationalOperator: 'Compare' (':5699:2')
         */
        if ((int32_T)B.Merge7 > 950)
        {
          /* Merge: 'Merge3' (':5684') incorporates:
           *  Constant: 'Constant1' (':5702')
           */
          Merge3 = 0U;
        }
        else
        {
          /* Merge: 'Merge3' (':5684') incorporates:
           *  Constant: 'Constant' (':5700:3')
           *  RelationalOperator: 'Compare' (':5700:2')
           */
          Merge3 = (uint16_T)((int32_T)B.Merge7 >= 100);
        }

        /* End of Switch: 'Switch' (':5703') */
        /* End of Outputs for SubSystem: 'Sector1' (':5696') */
        break;

       default:
        /* Outputs for IfAction SubSystem: 'Sector2' (':5706') incorporates:
         *  ActionPort: 'Action Port' (':5708')
         */
        /* Merge: 'Merge3' (':5684') incorporates:
         *  Constant: 'Constant' (':5709')
         *  SignalConversion generated from: 'SectorOut' (':5711')
         */
        Merge3 = 0U;

        /* End of Outputs for SubSystem: 'Sector2' (':5706') */
        break;
      }

      /* End of SwitchCase: 'Switch Case' (':5712') */

      /* RelationalOperator: 'Relational Operator' (':5663') incorporates:
       *  UnitDelay: 'Unit Delay' (':6036')
       */
      rtb_Rst = (DW.UnitDelay_DSTATE_bkn4 != Merge3);

      /* SwitchCase: 'Switch Case' (':5746') incorporates:
       *  MinMax: 'Min1' (':5784')
       */
      if ((int32_T)Merge3 == 0)
      {
        /* Outputs for IfAction SubSystem: 'Switch Case Action Subsystem' (':5747') incorporates:
         *  ActionPort: 'Action Port' (':5750')
         */
        /* SignalConversion generated from: 'Speed' (':5755') incorporates:
         *  Constant: 'Constant2' (':5752')
         */
        rtb_Speed = 0;

        /* SignalConversion generated from: 'DelayTime' (':5756') incorporates:
         *  Constant: 'Constant1' (':5751')
         */
        rtb_Delay = 0U;

        /* End of Outputs for SubSystem: 'Switch Case Action Subsystem' (':5747') */
      }
      else
      {
        /* Outputs for IfAction SubSystem: 'Switch Case Action Subsystem2' (':5767') incorporates:
         *  ActionPort: 'Action Port' (':5770')
         */
        if ((int32_T)B.Merge7 < 900)
        {
          /* MinMax: 'Min1' (':5784') */
          y = B.Merge7;
        }
        else
        {
          /* MinMax: 'Min1' (':5784') */
          y = 900U;
        }

        /* Outputs for Atomic SubSystem: 'MaxSpeedLimtedByVoltage' (':5648') */
        /* MinMax: 'Min' (':5783') incorporates:
         *  Constant: 'Constant' (':5651')
         *  DataTypeConversion: 'Data Type Conversion' (':5647')
         *  Inport: 'FilteredVoltage' (':5038')
         *  Lookup_n-D: '1-D Lookup Table' (':5650')
         *  Product: 'Divide' (':5653')
         */
        u0 = (int32_T)(int16_T)(int32_T)asr_s32((int32_T)(635 * (int32_T)
          look1_is16lu32n16Ds32_binlcs(U.FilteredVoltage,
          ConstP.uDLookupTable_bp01Data, ConstP.uDLookupTable_tableData, 2U)),
          6U);

        /* End of Outputs for SubSystem: 'MaxSpeedLimtedByVoltage' (':5648') */

        /* MinMax: 'Max' (':5782') */
        if ((int32_T)y > 100)
        {
        }
        else
        {
          y = 100U;
        }

        /* End of MinMax: 'Max' (':5782') */

        /* Sum: 'Add3' (':5774') incorporates:
         *  Constant: 'Constant2' (':5777')
         *  Constant: 'Constant3' (':5778')
         *  Product: 'Divide' (':5781')
         *  Product: 'Product' (':5785')
         *  Sum: 'Add' (':5771')
         *  Sum: 'Add1' (':5772')
         *  Sum: 'Add2' (':5773')
         */
        u1 = (int32_T)(div_nde_s32_floor((int32_T)((int32_T)((int32_T)y - 100) *
          179), 80) + 750);

        /* MinMax: 'Min' (':5783') */
        if (u0 < u1)
        {
          /* DataTypeConversion: 'Data Type Conversion' (':5780') */
          rtb_Speed = (int16_T)u0;
        }
        else
        {
          /* DataTypeConversion: 'Data Type Conversion' (':5780') */
          rtb_Speed = (int16_T)u1;
        }

        /* SignalConversion generated from: 'DelayTime' (':5787') incorporates:
         *  Constant: 'Constant1' (':5776')
         */
        rtb_Delay = 0U;

        /* End of Outputs for SubSystem: 'Switch Case Action Subsystem2' (':5767') */
      }

      /* End of SwitchCase: 'Switch Case' (':5746') */
      /* End of Outputs for SubSystem: 'SpeedCurve' (':5655') */

      /* SignalConversion generated from: 'SpeedCurveSectorNew' (':5801') */
      rtb_Sector = Merge3;

      /* End of Outputs for SubSystem: 'DutySpeedCtrl' (':5638') */
      break;

     default:
      /* Outputs for IfAction SubSystem: 'KeepLastState' (':5923') incorporates:
       *  ActionPort: 'Action Port' (':5927')
       */
      /* SignalConversion generated from: 'Rst' (':5931') incorporates:
       *  Constant: 'Constant' (':5928')
       */
      rtb_Rst = false;
      rtb_Speed = DW.LastSpeed_PreviousInput;
      rtb_Delay = DW.LastDelay_PreviousInput;
      rtb_Sector = DW.UnitDelay_DSTATE_bkn4;

      /* End of Outputs for SubSystem: 'KeepLastState' (':5923') */
      break;
    }

    /* End of If: 'If1' (':5922') */

    /* Outputs for Atomic SubSystem: 'SpeedDelayOut' (':6029') */
    SpeedDelayOut(rtb_Speed, rtb_Delay, rtb_Rst, &B.Out, &DW.SpeedDelayOut_p0zg);

    /* End of Outputs for SubSystem: 'SpeedDelayOut' (':6029') */

    /* Update for UnitDelay: 'Delay Input1' (':5636:2') incorporates:
     *  Inport: 'FilteredVoltage' (':5038')
     *
     * Block description for 'Delay Input1' (':5636:2'):
     *
     *  Store in Global RAM
     */
    DW.DelayInput1_DSTATE = U.FilteredVoltage;

    /* Update for Memory: 'DutyLast' (':5637') */
    DW.DutyLast_PreviousInput = B.Merge7;

    /* Update for Memory: 'LastSpeed' (':5934') */
    DW.LastSpeed_PreviousInput = rtb_Speed;

    /* Update for Memory: 'LastDelay' (':5933') */
    DW.LastDelay_PreviousInput = rtb_Delay;

    /* Update for UnitDelay: 'Unit Delay' (':6036') */
    DW.UnitDelay_DSTATE_bkn4 = rtb_Sector;

    /* End of Outputs for SubSystem: 'PwmInSpeedCtrl' (':5628') */

    /* Merge: 'Merge2' (':5620') incorporates:
     *  SignalConversion generated from: 'PwmInError' (':6041')
     */
    B.PwmInError = B.Merge;

    /* SignalConversion generated from: 'TargetSpeed' (':6040') */
    B.PwmInTargetSpeed = B.Out;

    /* End of Outputs for SubSystem: 'NormalState' (':5621') */
  }

  /* End of If: 'If' (':5617') */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
