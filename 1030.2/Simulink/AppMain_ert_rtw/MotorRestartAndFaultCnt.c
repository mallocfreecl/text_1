/*
 * File: MotorRestartAndFaultCnt.c
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

#include "MotorRestartAndFaultCnt.h"

/* Include model header file for global data */
#include "AppMain.h"
#include "AppMain_private.h"

/* Named constants for Chart: ''MotorRestartFSM' (':5428')' */
#define IN_NO_ACTIVE_CHILD_etkn        ((uint8_T)0U)
#define IN_ReStart                     ((uint8_T)1U)
#define IN_TimeCnt                     ((uint8_T)2U)

/*
 * Output and update for action system:
 *    'Short' (':5511')
 *    'Open' (':5501')
 *    'Lock' (':5491')
 */
void Short(uint8_T rtu_FaultCnt, uint16_T *rty_Interval)
{
  /* MultiPortSwitch: 'Multiport Switch' (':5518') incorporates:
   *  Constant: 'Constant3' (':5514')
   *  Constant: 'Constant4' (':5515')
   *  Constant: 'Constant5' (':5516')
   *  Constant: 'Constant6' (':5517')
   */
  switch ((int32_T)rtu_FaultCnt)
  {
   case 1:
    *rty_Interval = 500U;
    break;

   case 2:
    *rty_Interval = 1800U;
    break;

   case 3:
    *rty_Interval = 3300U;
    break;

   default:
    *rty_Interval = 6000U;
    break;
  }

  /* End of MultiPortSwitch: 'Multiport Switch' (':5518') */
}

/* System initialize for atomic system: 'MotorRestartAndFaultCnt' (':5328') */
void MotorRestartAndFaultCnt_Init(DW_MotorRestartAndFaultCnt_T *localDW)
{
  /* Start for If: 'If' (':5394') */
  localDW->If_ActiveSubsystem = -1;

  /* SystemInitialize for IfAction SubSystem: 'MotorFalutHandle' (':5406') */
  /* InitializeConditions for Memory: 'Memory' (':5424') */
  localDW->Memory_PreviousInput = 0U;

  /* InitializeConditions for Memory: 'RestartIntervalLast' (':5533') */
  localDW->RestartIntervalLast_PreviousInp = 0U;

  /* SystemInitialize for Atomic SubSystem: 'MotorFaultRecoverTimer' (':5425') */
  /* SystemInitialize for Chart: 'MotorRestartFSM' (':5428') */
  localDW->is_active_c1_AppMain = 0U;
  localDW->is_c1_AppMain = IN_NO_ACTIVE_CHILD_etkn;
  localDW->TimeCnt = 0U;

  /* End of SystemInitialize for SubSystem: 'MotorFaultRecoverTimer' (':5425') */
  /* End of SystemInitialize for SubSystem: 'MotorFalutHandle' (':5406') */
}

/* Output and update for atomic system: 'MotorRestartAndFaultCnt' (':5328') */
void MotorRestartAndFaultCnt(int16_T rtu_TargetSpeed, boolean_T rtu_ECUFault,
  boolean_T rtu_RestartComplete, boolean_T rtu_IsMotorShort, boolean_T
  rtu_IsMotorLock, boolean_T rtu_IsMotorOpenLoad, boolean_T
  rtu_IsMotorDryRunning, int16_T *rty_FaultSpeed, boolean_T *rty_FaultRestartEn,
  uint8_T *rty_ShortCnt, uint8_T *rty_OpenCnt, uint8_T *rty_LockCnt,
  DW_MotorRestartAndFaultCnt_T *localDW)
{
  int32_T tmp_0;
  uint32_T tmp;
  int16_T rtb_Merge_c3vs;
  uint16_T rtb_RestartInterval;
  int8_T rtAction;
  int8_T rtPrevAction;
  uint8_T rtb_FaultDetectedLast_Lock;
  uint8_T rtb_FaultDetectedLast_Open;
  uint8_T rtb_FaultDetectedLast_Short;
  uint8_T rtb_Short_fqg1;
  uint8_T rtb_Switch3_aizw;
  uint8_T rtb_Switch4_bqqu;

  /* If: 'If' (':5394') incorporates:
   *  Inport: 'FalutCntLast' (':5344')
   *  Inport: 'TargetSpeed' (':5563')
   *  Logic: 'Logical Operator4' (':5400')
   *  Memory: 'LockCntLast' (':5398')
   *  Memory: 'OpenCntLast' (':5575')
   *  Memory: 'ShortCntLast' (':5576')
   *  Merge: 'Merge1' (':5403')
   */
  rtPrevAction = localDW->If_ActiveSubsystem;
  if (rtu_ECUFault)
  {
    rtAction = 0;
  }
  else if (rtu_IsMotorShort || rtu_IsMotorLock || rtu_IsMotorOpenLoad)
  {
    rtAction = 1;
  }
  else
  {
    rtAction = 2;
  }

  localDW->If_ActiveSubsystem = rtAction;
  switch (rtAction)
  {
   case 0:
    /* Outputs for IfAction SubSystem: 'ECUFalutHandle' (':5343') incorporates:
     *  ActionPort: 'Action Port' (':5345')
     */
    /* SignalConversion generated from: 'Speed' (':5349') incorporates:
     *  Constant: 'Constant' (':5346')
     */
    rtb_Merge_c3vs = 0;

    /* SignalConversion generated from: 'FaultRestartEn' (':5350') incorporates:
     *  Constant: 'Constant2' (':5348')
     */
    *rty_FaultRestartEn = false;
    rtb_FaultDetectedLast_Short = localDW->ShortCntLast_PreviousInput;
    rtb_FaultDetectedLast_Open = localDW->OpenCntLast_PreviousInput;
    rtb_FaultDetectedLast_Lock = localDW->LockCntLast_PreviousInput;

    /* End of Outputs for SubSystem: 'ECUFalutHandle' (':5343') */
    break;

   case 1:
    if ((int32_T)rtAction != (int32_T)rtPrevAction)
    {
      /* InitializeConditions for IfAction SubSystem: 'MotorFalutHandle' (':5406') incorporates:
       *  ActionPort: 'Action Port' (':5414')
       */
      /* InitializeConditions for If: 'If' (':5394') incorporates:
       *  Memory: 'Memory' (':5424')
       *  Memory: 'RestartIntervalLast' (':5533')
       */
      localDW->Memory_PreviousInput = 0U;
      localDW->RestartIntervalLast_PreviousInp = 0U;

      /* End of InitializeConditions for SubSystem: 'MotorFalutHandle' (':5406') */

      /* SystemReset for IfAction SubSystem: 'MotorFalutHandle' (':5406') incorporates:
       *  ActionPort: 'Action Port' (':5414')
       */
      /* SystemReset for Atomic SubSystem: 'MotorFaultRecoverTimer' (':5425') */
      /* SystemReset for If: 'If' (':5394') incorporates:
       *  Chart: 'MotorRestartFSM' (':5428')
       */
      localDW->is_active_c1_AppMain = 0U;
      localDW->is_c1_AppMain = IN_NO_ACTIVE_CHILD_etkn;
      localDW->TimeCnt = 0U;

      /* End of SystemReset for SubSystem: 'MotorFaultRecoverTimer' (':5425') */
      /* End of SystemReset for SubSystem: 'MotorFalutHandle' (':5406') */
    }

    /* Outputs for IfAction SubSystem: 'MotorFalutHandle' (':5406') incorporates:
     *  ActionPort: 'Action Port' (':5414')
     */
    /* If: 'If' (':5439') incorporates:
     *  Constant: 'Constant' (':5416:3')
     *  Constant: 'Constant' (':5417:3')
     *  Inport: 'RestartIntervalLast' (':5525')
     *  Logic: 'Logical Operator' (':5422')
     *  Logic: 'Logical Operator1' (':5423')
     *  Memory: 'Memory' (':5424')
     *  Memory: 'RestartIntervalLast' (':5533')
     *  RelationalOperator: 'Compare' (':5416:2')
     *  RelationalOperator: 'Compare' (':5417:2')
     */
    if (((int32_T)localDW->Memory_PreviousInput == 0) || (((int32_T)
          rtu_TargetSpeed != 0) && rtu_RestartComplete))
    {
      /* Outputs for IfAction SubSystem: 'If Action Subsystem' (':5440') incorporates:
       *  ActionPort: 'Action Port' (':5445')
       */
      /* Switch: 'Switch' (':5481') incorporates:
       *  Constant: 'Constant1' (':5457')
       *  Sum: 'Add' (':5451')
       */
      if (rtu_IsMotorShort)
      {
        /* Sum: 'Add' (':5451') incorporates:
         *  Constant: 'Constant' (':5456')
         *  Memory: 'ShortCntLast' (':5576')
         */
        tmp_0 = (int32_T)(uint32_T)((uint32_T)
          localDW->ShortCntLast_PreviousInput + 1U);
        if ((uint32_T)((uint32_T)localDW->ShortCntLast_PreviousInput + 1U) >
            255U)
        {
          tmp_0 = 255;
        }

        rtb_Short_fqg1 = (uint8_T)tmp_0;
      }
      else
      {
        rtb_Short_fqg1 = 0U;
      }

      /* End of Switch: 'Switch' (':5481') */

      /* Switch: 'Switch3' (':5484') incorporates:
       *  Constant: 'Constant3' (':5459')
       */
      if (rtu_IsMotorOpenLoad)
      {
        /* Switch: 'Switch1' (':5482') incorporates:
         *  Memory: 'OpenCntLast' (':5575')
         *  Sum: 'Add3' (':5452')
         */
        if (rtu_IsMotorShort)
        {
          rtb_Switch3_aizw = localDW->OpenCntLast_PreviousInput;
        }
        else
        {
          /* Sum: 'Add3' (':5452') incorporates:
           *  Constant: 'Constant2' (':5458')
           *  Memory: 'OpenCntLast' (':5575')
           */
          tmp_0 = (int32_T)(uint32_T)((uint32_T)
            localDW->OpenCntLast_PreviousInput + 1U);
          if ((uint32_T)((uint32_T)localDW->OpenCntLast_PreviousInput + 1U) >
              255U)
          {
            tmp_0 = 255;
          }

          rtb_Switch3_aizw = (uint8_T)tmp_0;
        }

        /* End of Switch: 'Switch1' (':5482') */
      }
      else
      {
        rtb_Switch3_aizw = 0U;
      }

      /* End of Switch: 'Switch3' (':5484') */

      /* Switch: 'Switch4' (':5485') incorporates:
       *  Constant: 'Constant5' (':5461')
       */
      if (rtu_IsMotorLock)
      {
        /* Switch: 'Switch2' (':5483') incorporates:
         *  Logic: 'Logical Operator4' (':5480')
         *  Memory: 'LockCntLast' (':5398')
         *  Sum: 'Add4' (':5453')
         */
        if (rtu_IsMotorShort || rtu_IsMotorOpenLoad)
        {
          rtb_Switch4_bqqu = localDW->LockCntLast_PreviousInput;
        }
        else
        {
          /* Sum: 'Add4' (':5453') incorporates:
           *  Constant: 'Constant4' (':5460')
           *  Memory: 'LockCntLast' (':5398')
           */
          tmp_0 = (int32_T)(uint32_T)((uint32_T)
            localDW->LockCntLast_PreviousInput + 1U);
          if ((uint32_T)((uint32_T)localDW->LockCntLast_PreviousInput + 1U) >
              255U)
          {
            tmp_0 = 255;
          }

          rtb_Switch4_bqqu = (uint8_T)tmp_0;
        }

        /* End of Switch: 'Switch2' (':5483') */
      }
      else
      {
        rtb_Switch4_bqqu = 0U;
      }

      /* End of Switch: 'Switch4' (':5485') */

      /* SignalConversion generated from: 'FaultCnt' (':5521') incorporates:
       *  BusCreator: 'Bus Creator' (':5454')
       *  Merge: 'Merge1' (':5403')
       */
      rtb_FaultDetectedLast_Short = rtb_Short_fqg1;
      rtb_FaultDetectedLast_Open = rtb_Switch3_aizw;
      rtb_FaultDetectedLast_Lock = rtb_Switch4_bqqu;

      /* If: 'If' (':5490') incorporates:
       *  BusCreator: 'Bus Creator' (':5454')
       */
      if ((int32_T)rtb_Short_fqg1 > 0)
      {
        /* Outputs for IfAction SubSystem: 'Short' (':5511') incorporates:
         *  ActionPort: 'Action Port' (':5513')
         */
        /* SignalConversion generated from: 'RestartInterval' (':5522') */
        Short(rtb_Short_fqg1, &rtb_RestartInterval);

        /* End of Outputs for SubSystem: 'Short' (':5511') */
      }
      else if ((int32_T)rtb_Switch3_aizw > 0)
      {
        /* Outputs for IfAction SubSystem: 'Open' (':5501') incorporates:
         *  ActionPort: 'Action Port' (':5503')
         */
        /* SignalConversion generated from: 'RestartInterval' (':5522') */
        Short(rtb_Switch3_aizw, &rtb_RestartInterval);

        /* End of Outputs for SubSystem: 'Open' (':5501') */
      }
      else
      {
        /* Outputs for IfAction SubSystem: 'Lock' (':5491') incorporates:
         *  ActionPort: 'Action Port' (':5493')
         */
        /* SignalConversion generated from: 'RestartInterval' (':5522') */
        Short(rtb_Switch4_bqqu, &rtb_RestartInterval);

        /* End of Outputs for SubSystem: 'Lock' (':5491') */
      }

      /* End of If: 'If' (':5490') */
      /* End of Outputs for SubSystem: 'If Action Subsystem' (':5440') */
    }
    else
    {
      /* Outputs for IfAction SubSystem: 'If Action Subsystem1' (':5523') incorporates:
       *  ActionPort: 'Action Port' (':5526')
       */
      /* BusCreator: 'Bus Creator' (':5527') incorporates:
       *  Memory: 'LockCntLast' (':5398')
       *  Memory: 'OpenCntLast' (':5575')
       *  Memory: 'ShortCntLast' (':5576')
       *  Merge: 'Merge1' (':5403')
       */
      rtb_FaultDetectedLast_Short = localDW->ShortCntLast_PreviousInput;
      rtb_FaultDetectedLast_Open = localDW->OpenCntLast_PreviousInput;
      rtb_FaultDetectedLast_Lock = localDW->LockCntLast_PreviousInput;
      rtb_RestartInterval = localDW->RestartIntervalLast_PreviousInp;

      /* End of Outputs for SubSystem: 'If Action Subsystem1' (':5523') */
    }

    /* End of If: 'If' (':5439') */

    /* Outputs for Atomic SubSystem: 'MotorFaultRecoverTimer' (':5425') */
    /* Chart: 'MotorRestartFSM' (':5428') */
    /* Gateway: MotorRestartAndFaultCnt/MotorFalutHandle/MotorFaultRecoverTimer/MotorRestartFSM */
    /* During: MotorRestartAndFaultCnt/MotorFalutHandle/MotorFaultRecoverTimer/MotorRestartFSM */
    if ((uint32_T)localDW->is_active_c1_AppMain == 0U)
    {
      /* Entry: MotorRestartAndFaultCnt/MotorFalutHandle/MotorFaultRecoverTimer/MotorRestartFSM */
      localDW->is_active_c1_AppMain = 1U;

      /* Entry Internal: MotorRestartAndFaultCnt/MotorFalutHandle/MotorFaultRecoverTimer/MotorRestartFSM */
      /* Transition: (':5428:19') */
      localDW->is_c1_AppMain = IN_TimeCnt;

      /* Entry 'TimeCnt': (':5428:16') */
      localDW->TimeCnt = 1U;

      /* SignalConversion generated from: 'FaultRestartEn' (':5558') */
      *rty_FaultRestartEn = false;
    }
    else if (localDW->is_c1_AppMain == IN_ReStart)
    {
      /* SignalConversion generated from: 'FaultRestartEn' (':5558') */
      *rty_FaultRestartEn = true;

      /* During 'ReStart': (':5428:4') */
      if (rtu_RestartComplete)
      {
        /* Transition: (':5428:18') */
        localDW->is_c1_AppMain = IN_TimeCnt;

        /* Entry 'TimeCnt': (':5428:16') */
        localDW->TimeCnt = 1U;

        /* SignalConversion generated from: 'FaultRestartEn' (':5558') */
        *rty_FaultRestartEn = false;
      }
    }
    else
    {
      /* SignalConversion generated from: 'FaultRestartEn' (':5558') */
      *rty_FaultRestartEn = false;

      /* During 'TimeCnt': (':5428:16') */
      if ((localDW->TimeCnt >= rtb_RestartInterval) && ((int32_T)
           rtb_RestartInterval != 65535))
      {
        /* Transition: (':5428:17') */
        localDW->is_c1_AppMain = IN_ReStart;

        /* SignalConversion generated from: 'FaultRestartEn' (':5558') */
        /* Entry 'ReStart': (':5428:4') */
        *rty_FaultRestartEn = true;
      }
      else
      {
        tmp = (uint32_T)((uint32_T)localDW->TimeCnt + 1U);
        if ((uint32_T)((uint32_T)localDW->TimeCnt + 1U) > 65535U)
        {
          tmp = 65535U;
        }

        localDW->TimeCnt = (uint16_T)tmp;
      }
    }

    /* End of Chart: 'MotorRestartFSM' (':5428') */
    /* End of Outputs for SubSystem: 'MotorFaultRecoverTimer' (':5425') */

    /* Outputs for IfAction SubSystem: 'If Action Subsystem1' (':5546') incorporates:
     *  ActionPort: 'Action Port' (':5549')
     */
    /* Outputs for IfAction SubSystem: 'If Action Subsystem' (':5542') incorporates:
     *  ActionPort: 'Action Port' (':5544')
     */
    /* If: 'If' (':5541') incorporates:
     *  Inport: 'SpeedIn' (':5543')
     *  Inport: 'SpeedIn' (':5547')
     */
    rtb_Merge_c3vs = rtu_TargetSpeed;

    /* End of Outputs for SubSystem: 'If Action Subsystem' (':5542') */
    /* End of Outputs for SubSystem: 'If Action Subsystem1' (':5546') */

    /* Sum: 'Add' (':5415') incorporates:
     *  Constant: 'Constant' (':5418')
     *  Memory: 'Memory' (':5424')
     */
    tmp_0 = (int32_T)(uint32_T)((uint32_T)localDW->Memory_PreviousInput + 1U);
    if ((uint32_T)((uint32_T)localDW->Memory_PreviousInput + 1U) > 255U)
    {
      tmp_0 = 255;
    }

    /* Update for Memory: 'Memory' (':5424') incorporates:
     *  Sum: 'Add' (':5415')
     */
    localDW->Memory_PreviousInput = (uint8_T)tmp_0;

    /* Update for Memory: 'RestartIntervalLast' (':5533') */
    localDW->RestartIntervalLast_PreviousInp = rtb_RestartInterval;

    /* End of Outputs for SubSystem: 'MotorFalutHandle' (':5406') */
    break;

   default:
    /* Outputs for IfAction SubSystem: 'Normal' (':5562') incorporates:
     *  ActionPort: 'Action Port' (':5564')
     */
    /* SignalConversion generated from: 'FaultRestartEn' (':5572') incorporates:
     *  Constant: 'Constant2' (':5568')
     */
    *rty_FaultRestartEn = false;

    /* BusCreator: 'Bus Creator' (':5565') incorporates:
     *  Constant: 'Constant' (':5566')
     *  Constant: 'Constant3' (':5569')
     *  Constant: 'Constant4' (':5570')
     *  Merge: 'Merge1' (':5403')
     */
    rtb_FaultDetectedLast_Short = 0U;
    rtb_FaultDetectedLast_Open = 0U;
    rtb_FaultDetectedLast_Lock = 0U;
    rtb_Merge_c3vs = rtu_TargetSpeed;

    /* End of Outputs for SubSystem: 'Normal' (':5562') */
    break;
  }

  /* End of If: 'If' (':5394') */

  /* SignalConversion generated from: 'LockCnt' (':5583') incorporates:
   *  Memory: 'LockCntLast' (':5398')
   */
  *rty_LockCnt = localDW->LockCntLast_PreviousInput;

  /* SignalConversion generated from: 'OpenCnt' (':5582') incorporates:
   *  Memory: 'OpenCntLast' (':5575')
   */
  *rty_OpenCnt = localDW->OpenCntLast_PreviousInput;

  /* SignalConversion generated from: 'ShortCnt' (':5581') incorporates:
   *  Memory: 'ShortCntLast' (':5576')
   */
  *rty_ShortCnt = localDW->ShortCntLast_PreviousInput;

  /* Switch: 'Switch' (':5577') incorporates:
   *  Constant: 'Constant' (':5341')
   */
  if (rtu_IsMotorDryRunning)
  {
    *rty_FaultSpeed = 0;
  }
  else
  {
    *rty_FaultSpeed = rtb_Merge_c3vs;
  }

  /* End of Switch: 'Switch' (':5577') */

  /* Update for Memory: 'ShortCntLast' (':5576') incorporates:
   *  Merge: 'Merge1' (':5403')
   */
  localDW->ShortCntLast_PreviousInput = rtb_FaultDetectedLast_Short;

  /* Update for Memory: 'OpenCntLast' (':5575') incorporates:
   *  Merge: 'Merge1' (':5403')
   */
  localDW->OpenCntLast_PreviousInput = rtb_FaultDetectedLast_Open;

  /* Update for Memory: 'LockCntLast' (':5398') incorporates:
   *  Merge: 'Merge1' (':5403')
   */
  localDW->LockCntLast_PreviousInput = rtb_FaultDetectedLast_Lock;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
