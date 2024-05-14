/*
 * File: rt_sys_ECUFaultDiag_1.c
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

#include "rt_sys_ECUFaultDiag_1.h"

/* Include model header file for global data */
#include "AppMain.h"
#include "AppMain_private.h"

/* Named constants for Chart: ''AmbTOverDiag' (':5058')' */
#define IN_DefaultStatus               ((uint8_T)1U)
#define IN_High                        ((uint8_T)2U)
#define IN_Low                         ((uint8_T)3U)
#define IN_NO_ACTIVE_CHILD             ((uint8_T)0U)
#define IN_Normal                      ((uint8_T)4U)

/* Forward declaration for local functions */
static void UpdateStateCnt(int16_T lowTHR, int16_T highTHR, int16_T rtu_Input,
  DW_AmbTOverDiag_T *localDW);

/* Function for Chart: ''AmbTOverDiag' (':5058')' */
static void UpdateStateCnt(int16_T lowTHR, int16_T highTHR, int16_T rtu_Input,
  DW_AmbTOverDiag_T *localDW)
{
  uint32_T tmp;

  /* MATLAB Function 'UpdateStateCnt': (':6:39') */
  /* Graphical Function 'UpdateStateCnt': (':6:39') */
  /* Transition: (':6:41') */
  /* (':6:44:1') */
  if ((int32_T)rtu_Input < (int32_T)lowTHR)
  {
    /* Transition: (':6:44') */
    /* (':6:44:1') */
    localDW->HighCnt = 0U;

    /* (':6:44:1') */
    localDW->NormalCnt = 0U;

    /* (':6:97:1') */
    if ((int32_T)localDW->LowCnt < 65535)
    {
      /* Transition: (':6:97') */
      /* (':6:97:1') */
      tmp = (uint32_T)((uint32_T)localDW->LowCnt + 1U);
      if ((uint32_T)((uint32_T)localDW->LowCnt + 1U) > 65535U)
      {
        tmp = 65535U;
      }

      localDW->LowCnt = (uint16_T)tmp;
    }
    else
    {
      /* Transition: (':6:98') */
    }
  }
  else
  {
    /* Transition: (':6:46') */
    /* (':6:48:1') */
    if ((int32_T)rtu_Input > (int32_T)highTHR)
    {
      /* Transition: (':6:48') */
      /* (':6:48:1') */
      localDW->LowCnt = 0U;

      /* (':6:48:1') */
      localDW->NormalCnt = 0U;

      /* (':6:100:1') */
      if ((int32_T)localDW->HighCnt < 65535)
      {
        /* Transition: (':6:100') */
        /* (':6:100:1') */
        tmp = (uint32_T)((uint32_T)localDW->HighCnt + 1U);
        if ((uint32_T)((uint32_T)localDW->HighCnt + 1U) > 65535U)
        {
          tmp = 65535U;
        }

        localDW->HighCnt = (uint16_T)tmp;
      }
      else
      {
        /* Transition: (':6:101') */
      }
    }
    else
    {
      /* Transition: (':6:50') */
      /* Transition: (':6:52') */
      /* (':6:52:1') */
      localDW->LowCnt = 0U;

      /* (':6:52:1') */
      localDW->HighCnt = 0U;

      /* (':6:103:1') */
      if ((int32_T)localDW->NormalCnt < 65535)
      {
        /* Transition: (':6:103') */
        /* (':6:103:1') */
        tmp = (uint32_T)((uint32_T)localDW->NormalCnt + 1U);
        if ((uint32_T)((uint32_T)localDW->NormalCnt + 1U) > 65535U)
        {
          tmp = 65535U;
        }

        localDW->NormalCnt = (uint16_T)tmp;
      }
      else
      {
        /* Transition: (':6:104') */
      }

      /* Transition: (':6:57') */
    }

    /* Transition: (':6:58') */
  }

  /* Transition: (':6:56') */
}

/*
 * System initialize for atomic system:
 *    'AmbTOverDiag' (':5058')
 *    'VoltageDiag' (':5065')
 *    'AmbTPreOverDiag' (':6068')
 */
void AmbTOverDiag_Init(int8_T *rty_Status, DW_AmbTOverDiag_T *localDW)
{
  localDW->is_active_c2_lib1 = 0U;
  localDW->is_c2_lib1 = IN_NO_ACTIVE_CHILD;
  localDW->LowCnt = 0U;
  localDW->HighCnt = 0U;
  localDW->NormalCnt = 0U;
  localDW->DefaultStatus = 0;
  localDW->IsInited = false;
  *rty_Status = 0;
}

/*
 * Output and update for atomic system:
 *    'AmbTOverDiag' (':5058')
 *    'VoltageDiag' (':5065')
 *    'AmbTPreOverDiag' (':6068')
 */
void AmbTOverDiag(int16_T rtu_Input, int8_T *rty_Status, uint16_T
                  rtp_DelayCntTHR, int16_T rtp_HighEntryTHR, int16_T
                  rtp_HighExitTHR, int16_T rtp_LowEntryTHR, int16_T
                  rtp_LowExitTHR, DW_AmbTOverDiag_T *localDW)
{
  int32_T tmp;

  /* Chart: 'AmbTOverDiag' (':5058') */
  /* Gateway: HysteresisState */
  /* During: HysteresisState */
  if ((uint32_T)localDW->is_active_c2_lib1 == 0U)
  {
    /* Entry: HysteresisState */
    localDW->is_active_c2_lib1 = 1U;

    /* Entry Internal: HysteresisState */
    /* Transition: (':6:17') */
    localDW->is_c2_lib1 = IN_DefaultStatus;

    /* Entry 'DefaultStatus': (':6:65') */
    localDW->IsInited = false;
  }
  else
  {
    switch (localDW->is_c2_lib1)
    {
     case IN_DefaultStatus:
      /* During 'DefaultStatus': (':6:65') */
      if (localDW->IsInited)
      {
        /* Transition: (':6:90') */
        switch ((int32_T)localDW->DefaultStatus)
        {
         case 0:
          /* Transition: (':6:66') */
          localDW->is_c2_lib1 = IN_Normal;

          /* Entry 'Normal': (':6:16') */
          *rty_Status = 0;
          break;

         case -1:
          /* Transition: (':6:68') */
          /* Transition: (':6:69') */
          localDW->is_c2_lib1 = IN_Low;

          /* Entry 'Low': (':6:20') */
          *rty_Status = -1;
          break;

         default:
          /* Transition: (':6:71') */
          /* Transition: (':6:74') */
          /* Transition: (':6:73') */
          localDW->is_c2_lib1 = IN_High;

          /* Entry 'High': (':6:23') */
          *rty_Status = 1;
          break;
        }
      }
      else
      {
        /* Transition: (':6:79') */
        if ((int32_T)rtu_Input > (int32_T)rtp_HighEntryTHR)
        {
          /* Transition: (':6:77') */
          localDW->DefaultStatus = 1;
        }
        else
        {
          /* Transition: (':6:81') */
          if ((int32_T)rtu_Input < (int32_T)rtp_LowEntryTHR)
          {
            /* Transition: (':6:83') */
            localDW->DefaultStatus = -1;
          }
          else
          {
            /* Transition: (':6:85') */
            /* Transition: (':6:87') */
            localDW->DefaultStatus = 0;

            /* Transition: (':6:92') */
          }

          /* Transition: (':6:93') */
        }

        /* Transition: (':6:94') */
        *rty_Status = localDW->DefaultStatus;
        localDW->IsInited = true;
      }
      break;

     case IN_High:
      /* During 'High': (':6:23') */
      if (((int32_T)rtu_Input < (int32_T)rtp_LowEntryTHR) && (localDW->LowCnt >=
           rtp_DelayCntTHR))
      {
        /* Transition: (':6:30') */
        /* Transition: (':6:31') */
        localDW->is_c2_lib1 = IN_Low;

        /* Entry 'Low': (':6:20') */
        *rty_Status = -1;
      }
      else if (((int32_T)rtu_Input < (int32_T)rtp_HighExitTHR) &&
               (localDW->NormalCnt >= rtp_DelayCntTHR))
      {
        /* Transition: (':6:25') */
        localDW->is_c2_lib1 = IN_Normal;

        /* Entry 'Normal': (':6:16') */
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
      /* During 'Low': (':6:20') */
      if (((int32_T)rtu_Input > (int32_T)rtp_HighEntryTHR) && (localDW->HighCnt >=
           rtp_DelayCntTHR))
      {
        /* Transition: (':6:27') */
        /* Transition: (':6:28') */
        localDW->is_c2_lib1 = IN_High;

        /* Entry 'High': (':6:23') */
        *rty_Status = 1;
      }
      else if (((int32_T)rtu_Input > (int32_T)rtp_LowExitTHR) &&
               (localDW->NormalCnt >= rtp_DelayCntTHR))
      {
        /* Transition: (':6:21') */
        localDW->is_c2_lib1 = IN_Normal;

        /* Entry 'Normal': (':6:16') */
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
      /* During 'Normal': (':6:16') */
      if (((int32_T)rtu_Input < (int32_T)rtp_LowEntryTHR) && (localDW->LowCnt >=
           rtp_DelayCntTHR))
      {
        /* Transition: (':6:22') */
        localDW->is_c2_lib1 = IN_Low;

        /* Entry 'Low': (':6:20') */
        *rty_Status = -1;
      }
      else if (((int32_T)rtu_Input > (int32_T)rtp_HighEntryTHR) &&
               (localDW->HighCnt >= rtp_DelayCntTHR))
      {
        /* Transition: (':6:24') */
        localDW->is_c2_lib1 = IN_High;

        /* Entry 'High': (':6:23') */
        *rty_Status = 1;
      }
      else
      {
        UpdateStateCnt(rtp_LowEntryTHR, rtp_HighEntryTHR, rtu_Input, localDW);
      }
      break;
    }
  }

  /* End of Chart: 'AmbTOverDiag' (':5058') */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
