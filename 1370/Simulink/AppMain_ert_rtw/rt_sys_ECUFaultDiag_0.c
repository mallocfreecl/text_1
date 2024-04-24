/*
 * File: rt_sys_ECUFaultDiag_0.c
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

#include "rt_sys_ECUFaultDiag_0.h"

/* Include model header file for global data */
#include "AppMain.h"
#include "AppMain_private.h"

/* Named constants for Chart: ''TemperaturState' (':3117')' */
#define IN_DefaultStatus               ((uint8_T)1U)
#define IN_High                        ((uint8_T)2U)
#define IN_Low                         ((uint8_T)3U)
#define IN_NO_ACTIVE_CHILD             ((uint8_T)0U)
#define IN_Normal                      ((uint8_T)4U)

/* Forward declaration for local functions */
static void UpdateStateCnt(int16_T lowTHR, int16_T highTHR, int16_T rtu_Input,
  DW_TemperaturState_T *localDW);

/* Function for Chart: ''TemperaturState' (':3117')' */
static void UpdateStateCnt(int16_T lowTHR, int16_T highTHR, int16_T rtu_Input,
  DW_TemperaturState_T *localDW)
{
  uint32_T tmp;

  /* MATLAB Function 'UpdateStateCnt': (':32:39') */
  /* Graphical Function 'UpdateStateCnt': (':32:39') */
  /* Transition: (':32:41') */
  /* (':32:44:1') */
  if ((int32_T)rtu_Input < (int32_T)lowTHR)
  {
    /* Transition: (':32:44') */
    /* (':32:44:1') */
    tmp = (uint32_T)((uint32_T)localDW->LowCnt + 1U);
    if ((uint32_T)((uint32_T)localDW->LowCnt + 1U) > 65535U)
    {
      tmp = 65535U;
    }

    localDW->LowCnt = (uint16_T)tmp;

    /* (':32:44:1') */
    localDW->HighCnt = 0U;

    /* (':32:44:3') */
    localDW->NormalCnt = 0U;
  }
  else
  {
    /* Transition: (':32:46') */
    /* (':32:48:1') */
    if ((int32_T)rtu_Input > (int32_T)highTHR)
    {
      /* Transition: (':32:48') */
      /* (':32:48:1') */
      localDW->LowCnt = 0U;

      /* (':32:48:1') */
      tmp = (uint32_T)((uint32_T)localDW->HighCnt + 1U);
      if ((uint32_T)((uint32_T)localDW->HighCnt + 1U) > 65535U)
      {
        tmp = 65535U;
      }

      localDW->HighCnt = (uint16_T)tmp;

      /* (':32:48:3') */
      localDW->NormalCnt = 0U;
    }
    else
    {
      /* Transition: (':32:50') */
      /* Transition: (':32:52') */
      /* (':32:52:1') */
      localDW->LowCnt = 0U;

      /* (':32:52:1') */
      localDW->HighCnt = 0U;

      /* (':32:52:1') */
      tmp = (uint32_T)((uint32_T)localDW->NormalCnt + 1U);
      if ((uint32_T)((uint32_T)localDW->NormalCnt + 1U) > 65535U)
      {
        tmp = 65535U;
      }

      localDW->NormalCnt = (uint16_T)tmp;

      /* Transition: (':32:57') */
    }

    /* Transition: (':32:58') */
  }

  /* Transition: (':32:56') */
}

/*
 * System initialize for atomic system:
 *    'TemperaturState' (':3117')
 *    'AmbTPreOverDiag' (':3119')
 */
void TemperaturState_Init(int8_T *rty_Status, DW_TemperaturState_T *localDW)
{
  localDW->is_active_c1_lib1 = 0U;
  localDW->is_c1_lib1 = IN_NO_ACTIVE_CHILD;
  localDW->LowCnt = 0U;
  localDW->HighCnt = 0U;
  localDW->NormalCnt = 0U;
  localDW->DefaultStatus = 0;
  localDW->IsInited = false;
  *rty_Status = 0;
}

/*
 * Output and update for atomic system:
 *    'TemperaturState' (':3117')
 *    'AmbTPreOverDiag' (':3119')
 */
void TemperaturState(int16_T rtu_Input, int8_T *rty_Status, uint16_T
                     rtp_DelayCntTHR, int16_T rtp_HighEntryTHR, int16_T
                     rtp_HighExitTHR, uint16_T rtp_LongDelayCntTHR, int16_T
                     rtp_LowEntryTHR, int16_T rtp_LowExitTHR,
                     DW_TemperaturState_T *localDW)
{
  int32_T tmp;

  /* Chart: 'TemperaturState' (':3117') */
  /* Gateway: TemperaturSstate */
  /* During: TemperaturSstate */
  if ((uint32_T)localDW->is_active_c1_lib1 == 0U)
  {
    /* Entry: TemperaturSstate */
    localDW->is_active_c1_lib1 = 1U;

    /* Entry Internal: TemperaturSstate */
    /* Transition: (':32:17') */
    localDW->is_c1_lib1 = IN_DefaultStatus;

    /* Entry 'DefaultStatus': (':32:65') */
    localDW->IsInited = false;
  }
  else
  {
    switch (localDW->is_c1_lib1)
    {
     case IN_DefaultStatus:
      /* During 'DefaultStatus': (':32:65') */
      if (localDW->IsInited)
      {
        /* Transition: (':32:90') */
        switch ((int32_T)localDW->DefaultStatus)
        {
         case 0:
          /* Transition: (':32:66') */
          localDW->is_c1_lib1 = IN_Normal;

          /* Entry 'Normal': (':32:16') */
          *rty_Status = 0;
          break;

         case -1:
          /* Transition: (':32:68') */
          /* Transition: (':32:69') */
          localDW->is_c1_lib1 = IN_Low;

          /* Entry 'Low': (':32:20') */
          *rty_Status = -1;
          break;

         default:
          /* Transition: (':32:71') */
          /* Transition: (':32:74') */
          /* Transition: (':32:73') */
          localDW->is_c1_lib1 = IN_High;

          /* Entry 'High': (':32:23') */
          *rty_Status = 1;
          break;
        }
      }
      else
      {
        /* Transition: (':32:79') */
        if ((int32_T)rtu_Input > (int32_T)rtp_HighEntryTHR)
        {
          /* Transition: (':32:77') */
          localDW->DefaultStatus = 1;
        }
        else
        {
          /* Transition: (':32:81') */
          if ((int32_T)rtu_Input < (int32_T)rtp_LowEntryTHR)
          {
            /* Transition: (':32:83') */
            localDW->DefaultStatus = -1;
          }
          else
          {
            /* Transition: (':32:85') */
            /* Transition: (':32:87') */
            localDW->DefaultStatus = 0;

            /* Transition: (':32:92') */
          }

          /* Transition: (':32:93') */
        }

        /* Transition: (':32:94') */
        *rty_Status = localDW->DefaultStatus;
        localDW->IsInited = true;
      }
      break;

     case IN_High:
      /* During 'High': (':32:23') */
      if (((int32_T)rtu_Input < (int32_T)rtp_LowEntryTHR) && (localDW->LowCnt >=
           rtp_DelayCntTHR))
      {
        /* Transition: (':32:30') */
        /* Transition: (':32:31') */
        localDW->is_c1_lib1 = IN_Low;

        /* Entry 'Low': (':32:20') */
        *rty_Status = -1;
      }
      else if (((int32_T)rtu_Input < (int32_T)rtp_HighExitTHR) &&
               (localDW->NormalCnt >= rtp_LongDelayCntTHR))
      {
        /* Transition: (':32:25') */
        localDW->is_c1_lib1 = IN_Normal;

        /* Entry 'Normal': (':32:16') */
        *rty_Status = 0;
      }
      else
      {
        tmp = (int32_T)((int32_T)rtp_HighExitTHR - 1);
        if ((int32_T)((int32_T)rtp_HighExitTHR - 1) < -32768)
        {
          tmp = -32768;
        }

        UpdateStateCnt(rtp_LowEntryTHR, (int16_T)tmp, rtu_Input, localDW);
      }
      break;

     case IN_Low:
      /* During 'Low': (':32:20') */
      if (((int32_T)rtu_Input > (int32_T)rtp_HighEntryTHR) && (localDW->HighCnt >=
           rtp_DelayCntTHR))
      {
        /* Transition: (':32:27') */
        /* Transition: (':32:28') */
        localDW->is_c1_lib1 = IN_High;

        /* Entry 'High': (':32:23') */
        *rty_Status = 1;
      }
      else if (((int32_T)rtu_Input > (int32_T)rtp_LowExitTHR) &&
               (localDW->NormalCnt >= rtp_LongDelayCntTHR))
      {
        /* Transition: (':32:21') */
        localDW->is_c1_lib1 = IN_Normal;

        /* Entry 'Normal': (':32:16') */
        *rty_Status = 0;
      }
      else
      {
        tmp = (int32_T)((int32_T)rtp_LowExitTHR + 1);
        if ((int32_T)((int32_T)rtp_LowExitTHR + 1) > 32767)
        {
          tmp = 32767;
        }

        UpdateStateCnt((int16_T)tmp, rtp_HighEntryTHR, rtu_Input, localDW);
      }
      break;

     default:
      /* During 'Normal': (':32:16') */
      if (((int32_T)rtu_Input < (int32_T)rtp_LowEntryTHR) && (localDW->LowCnt >=
           rtp_DelayCntTHR))
      {
        /* Transition: (':32:22') */
        localDW->is_c1_lib1 = IN_Low;

        /* Entry 'Low': (':32:20') */
        *rty_Status = -1;
      }
      else if (((int32_T)rtu_Input > (int32_T)rtp_HighEntryTHR) &&
               (localDW->HighCnt >= rtp_DelayCntTHR))
      {
        /* Transition: (':32:24') */
        localDW->is_c1_lib1 = IN_High;

        /* Entry 'High': (':32:23') */
        *rty_Status = 1;
      }
      else
      {
        UpdateStateCnt(rtp_LowEntryTHR, rtp_HighEntryTHR, rtu_Input, localDW);
      }
      break;
    }
  }

  /* End of Chart: 'TemperaturState' (':3117') */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
