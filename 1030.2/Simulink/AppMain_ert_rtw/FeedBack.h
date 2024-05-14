/*
 * File: FeedBack.h
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

#ifndef RTW_HEADER_FeedBack_h_
#define RTW_HEADER_FeedBack_h_
#ifndef AppMain_COMMON_INCLUDES_
#define AppMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AppMain_COMMON_INCLUDES_ */

#include "AppMain_types.h"

/* Block states (default storage) for system 'DelayOut' (':5103:23') */
typedef struct
{
  int16_T DelayInput1_DSTATE;          /* 'Delay Input1' (':5103:23:13:2') */
  uint16_T DelayInput1_DSTATE_onhg;    /* 'Delay Input1' (':5103:23:12:2') */
  int16_T OutLast_PreviousInput;       /* 'OutLast' (':5103:23:15') */
  uint16_T DelayCnt;                   /* 'DelayOut' (':5103:23:11') */
  uint8_T is_active_c3_lib1;           /* 'DelayOut' (':5103:23:11') */
  uint8_T is_c3_lib1;                  /* 'DelayOut' (':5103:23:11') */
}
DW_DelayOut_T;

/* Block signals for system 'PowerUpBlank' (':5103') */
typedef struct
{
  int16_T Out;                         /* 'DelayOut' (':5103:23:11') */
}
B_FeedBackDelay_T;

/* Block states (default storage) for system 'PowerUpBlank' (':5103') */
typedef struct
{
  uint16_T Memory_PreviousInput;       /* 'Memory' (':5103:26') */
  boolean_T DelayInput1_DSTATE;        /* 'Delay Input1' (':5103:25:2') */
  boolean_T DelayInput1_DSTATE_l1yc;   /* 'Delay Input1' (':5103:24:2') */
  DW_DelayOut_T DelayOut_b34q;         /* 'DelayOut' (':5103:23') */
}
DW_FeedBackDelay_T;

/* Block states (default storage) for system 'DelayOut' (':5218:23') */
typedef struct
{
  int16_T DelayInput1_DSTATE;          /* 'Delay Input1' (':5218:23:13:2') */
  uint16_T DelayInput1_DSTATE_fqr4;    /* 'Delay Input1' (':5218:23:12:2') */
  int16_T OutLast_PreviousInput;       /* 'OutLast' (':5218:23:15') */
  uint16_T DelayCnt;                   /* 'DelayOut' (':5218:23:11') */
  uint8_T is_active_c3_lib1;           /* 'DelayOut' (':5218:23:11') */
  uint8_T is_c3_lib1;                  /* 'DelayOut' (':5218:23:11') */
}
DW_DelayOut_ahel_T;

/* Block signals for system 'FeedBackDelay4' (':5218') */
typedef struct
{
  int16_T Out;                         /* 'DelayOut' (':5218:23:11') */
}
B_FeedBackDelay_fu1b_T;

/* Block states (default storage) for system 'FeedBackDelay4' (':5218') */
typedef struct
{
  uint16_T Memory_PreviousInput;       /* 'Memory' (':5218:26') */
  boolean_T DelayInput1_DSTATE;        /* 'Delay Input1' (':5218:25:2') */
  boolean_T DelayInput1_DSTATE_evfm;   /* 'Delay Input1' (':5218:24:2') */
  DW_DelayOut_ahel_T DelayOut_kj4v5;   /* 'DelayOut' (':5218:23') */
}
DW_FeedBackDelay_lh44_T;

extern void DelayOut_Init(int16_T *rty_Out, DW_DelayOut_T *localDW);
extern void DelayOut(int16_T rtu_In, uint16_T rtu_DelayTime, int16_T *rty_Out,
                     DW_DelayOut_T *localDW);
extern void FeedBackDelay_Init(B_FeedBackDelay_T *localB, DW_FeedBackDelay_T
  *localDW);
extern boolean_T FeedBackDelay(uint16_T rtu_FeedBackStartDelayTime, boolean_T
  rtu_IsFault, uint16_T rtu_FeedBackEndDelayTime, B_FeedBackDelay_T *localB,
  DW_FeedBackDelay_T *localDW);
extern void DelayOut_pw2c_Init(int16_T *rty_Out, DW_DelayOut_ahel_T *localDW);
extern void DelayOut_kj4v(int16_T rtu_In, uint16_T rtu_DelayTime, int16_T
  *rty_Out, DW_DelayOut_ahel_T *localDW);
extern void FeedBackDelay4_Init(B_FeedBackDelay_fu1b_T *localB,
  DW_FeedBackDelay_lh44_T *localDW);
extern boolean_T FeedBackDelay4(uint16_T rtu_FeedBackStartDelayTime, boolean_T
  rtu_IsFault, uint16_T rtu_FeedBackEndDelayTime, B_FeedBackDelay_fu1b_T *localB,
  DW_FeedBackDelay_lh44_T *localDW);
extern void FeedBack_Init(void);
extern void FeedBack(void);

#endif                                 /* RTW_HEADER_FeedBack_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
