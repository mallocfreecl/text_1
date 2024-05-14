/*
 * File: AppMain.h
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

#ifndef RTW_HEADER_AppMain_h_
#define RTW_HEADER_AppMain_h_
#include <string.h>
#ifndef AppMain_COMMON_INCLUDES_
#define AppMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AppMain_COMMON_INCLUDES_ */

#include "AppMain_types.h"

/* Child system includes */
#include "ECUFaultDiag.h"
#include "FeedBack.h"
#include "MotorRestartAndFaultCnt.h"
#include "MotorTargetSpeedMux.h"
#include "PwmInParamEval.h"
#include "PwmInTargetSpeedEval.h"
#include "SleepMode.h"
#include "rt_sys_ECUFaultDiag_1.h"

/* Macros for accessing real-time model data structure */

/* Block signals (default storage) */
typedef struct
{
  uint16_T DutyLast;                   /* 'DutyLast' (':5637') */
  uint16_T Merge7;                     /* 'Merge7' (':5943') */
  uint16_T Switch;                     /* 'Switch' (':6535') */
  uint16_T Freq;
  /* 'TmpSignal ConversionAtBusConversion_InsertedFor_PwmInStatus_at_inport_0_BusSelector1Outport3' (':0') */
  uint16_T HighDuty;
  /* 'TmpSignal ConversionAtBusConversion_InsertedFor_PwmInStatus_at_inport_0_BusSelector1Outport4' (':0') */
  uint16_T Duty;                       /* 'MATLAB Function' (':5047') */
  int16_T PwmInTargetSpeed;            /* 'Merge1' (':5619') */
  int16_T Out;                         /* 'DelayOut' (':6033') */
  int8_T Status_mbnx;                  /* 'VoltageDiag' (':5065') */
  int8_T Status_dotr;                  /* 'AmbTOverDiag' (':5058') */
  boolean_T Compare;                   /* 'Compare' (':5051:2') */
  boolean_T GoToSleep;                 /* 'SleepMode' (':6059') */
  boolean_T ChipSleep;                 /* 'SleepMode' (':6059') */
  boolean_T ChipReset;                 /* 'SleepMode' (':6059') */
  boolean_T PwmInError;                /* 'Merge2' (':5620') */
  boolean_T Merge5;                    /* 'Merge5' (':5941') */
  boolean_T Merge;                     /* 'Merge' (':5937') */
  boolean_T DutyValid;                 /* 'Logical Operator' (':5985') */
  boolean_T State;                     /* 'StateKeep' (':5106') */
  boolean_T ECUFault;                  /* 'Logical Operator' (':5062') */
  B_DutyFilter_T DutyFilter_aldi;      /* 'DutyFilter' (':5951') */
  B_FeedBackDelay_fu1b_T FeedBackDelay4_ii3r;/* 'FeedBackDelay4' (':5218') */
  B_FeedBackDelay_fu1b_T FeedBackDelay4_j5pn;/* 'FeedBackDelay4' (':5218') */
  B_FeedBackDelay_fu1b_T FeedBackDelay4_cyog;/* 'FeedBackDelay4' (':5218') */
  B_FeedBackDelay_T FeedBackDelay4_lhnv;/* 'PowerUpBlank' (':5103') */
  B_FeedBackDelay_T FeedBackDelay4_eqch;/* 'PowerUpBlank' (':5103') */
  B_FeedBackDelay_T FeedBackDelay4_ls1m;/* 'PowerUpBlank' (':5103') */
  B_FeedBackDelay_T PowerUpBlank;      /* 'PowerUpBlank' (':5103') */
}
B_T;

/* Block states (default storage) for system 'AppMain' */
typedef struct
{
  int16_T DelayInput1_DSTATE;          /* 'Delay Input1' (':5636:2') */
  uint16_T UnitDelay_DSTATE;           /* 'Unit Delay' (':5048') */
  uint16_T UnitDelay_DSTATE_bkn4;      /* 'Unit Delay' (':6036') */
  uint16_T DelayInput1_DSTATE_dtn5;    /* 'Delay Input1' (':6018:2') */
  int16_T LastSpeed_PreviousInput;     /* 'LastSpeed' (':5934') */
  uint16_T PowerOnBlankTimeCnt_PreviousInp;/* 'PowerOnBlankTimeCnt' (':6052') */
  uint16_T DutyLast_PreviousInput;     /* 'DutyLast' (':5637') */
  uint16_T LastDelay_PreviousInput;    /* 'LastDelay' (':5933') */
  uint16_T Memory_PreviousInput;       /* 'Memory' (':5831') */
  uint16_T FalseTime;                  /* 'StateKeep' (':5106') */
  uint16_T TrueTime;                   /* 'StateKeep' (':5106') */
  boolean_T DelayInput1_DSTATE_evnj;   /* 'Delay Input1' (':5950:2') */
  boolean_T DelayInput1_DSTATE_ovaq;   /* 'Delay Input1' (':6019:2') */
  boolean_T DelayInput1_DSTATE_egdl;   /* 'Delay Input1' (':6020:2') */
  int8_T If_ActiveSubsystem;           /* 'If' (':5617') */
  int8_T If_ActiveSubsystem_dy0c;      /* 'If' (':5921') */
  int8_T If1_ActiveSubsystem;          /* 'If1' (':5922') */
  uint8_T is_active_c2_AppMain;        /* 'StateKeep' (':5106') */
  uint8_T is_c2_AppMain;               /* 'StateKeep' (':5106') */
  DW_AmbTOverDiag_T sf_AmbTPreOverDiag;/* 'AmbTOverDiag' (':5058') */
  DW_SleepMode_T SleepMode_h3wm;       /* 'SleepMode' (':6057') */
  DW_SpeedDelayOut_T SpeedDelayOut_p0zg;/* 'SpeedDelayOut' (':6029') */
  DW_FreqValidJudge_T FreqValidJudge_h5v4;/* 'FreqValidJudge' (':5997') */
  DW_DutyFilter_T DutyFilter_aldi;     /* 'DutyFilter' (':5951') */
  DW_MotorRestartAndFaultCnt_T MotorRestartAndFaultCnt_j4bv;/* 'MotorRestartAndFaultCnt' (':5328') */
  DW_FeedBackDelay_lh44_T FeedBackDelay4_ii3r;/* 'FeedBackDelay4' (':5218') */
  DW_FeedBackDelay_lh44_T FeedBackDelay4_j5pn;/* 'FeedBackDelay4' (':5218') */
  DW_FeedBackDelay_lh44_T FeedBackDelay4_cyog;/* 'FeedBackDelay4' (':5218') */
  DW_FeedBackDelay_T FeedBackDelay4_lhnv;/* 'PowerUpBlank' (':5103') */
  DW_FeedBackDelay_T FeedBackDelay4_eqch;/* 'PowerUpBlank' (':5103') */
  DW_FeedBackDelay_T FeedBackDelay4_ls1m;/* 'PowerUpBlank' (':5103') */
  DW_FeedBackDelay_T PowerUpBlank;     /* 'PowerUpBlank' (':5103') */
  DW_ECUFaultDiag_T ECUFaultDiag_gulw; /* 'ECUFaultDiag' (':5052') */
}
DW_T;

/* Constant parameters (default storage) */
typedef struct
{
  /* Expression: int16([256 256 256])
   * Referenced by: '1-D Lookup Table' (':5650')
   */
  int16_T uDLookupTable_tableData[3];

  /* Expression: int16([170 240 320])
   * Referenced by: '1-D Lookup Table' (':5650')
   */
  int16_T uDLookupTable_bp01Data[3];

  /* Expression: CtrlConst.FeedBack.LowTime
   * Referenced by: 'LowTime' (':5151')
   */
  uint16_T LowTime_table[7];

  /* Expression: CtrlConst.FeedBack.HighTime
   * Referenced by: 'HighTime' (':5150')
   */
  uint16_T HighTime_table[7];
}
ConstP_T;

/* External inputs (root inport signals with default storage) */
typedef struct
{
  int16_T SupVoltage;                  /* 'SupVoltage' (':5026') */
  int16_T AmbTemperature;              /* 'AmbTemperature' (':5027') */
  boolean_T ChipFault;                 /* 'ChipFault' (':5028') */
  boolean_T PwmInPinSts;               /* 'PwmInPinSts' (':5029') */
  boolean_T SleepValid;                /* 'SleepValid' (':5030') */
  uint16_T PwmInFreq;                  /* 'PwmInFreq' (':5031') */
  uint16_T PwmInDuty;                  /* 'PwmInDuty' (':5032') */
  uint16_T PwmInIdleTime;              /* 'PwmInIdleTime' (':5033') */
  boolean_T IsMotorShort;              /* 'IsMotorShort' (':5034') */
  boolean_T IsMotorLock;               /* 'IsMotorLock' (':5035') */
  boolean_T IsMotorOpenLoad;           /* 'IsMotorOpenLoad' (':5036') */
  boolean_T RestartComplete;           /* 'RestartComplete' (':5037') */
  int16_T FilteredVoltage;             /* 'FilteredVoltage' (':5038') */
  boolean_T IsMotorDryRunning;         /* 'IsMotorDryRunning' (':5039') */
  boolean_T TestModeSpeedCtrlEnable;   /* 'TestModeSpeedCtrlEnable' (':5040') */
  int16_T TestModeSpeed;               /* 'TestModeSpeed' (':5041') */
  boolean_T TestModeSleepEn;           /* 'TestModeSleepEn' (':5042') */
  uint16_T AnlogPinVoltage;            /* 'AnlogPinVoltage' (':5043') */
}
ExtU_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct
{
  boolean_T MotorRestartEnable;        /* 'MotorRestartEnable' (':6076') */
  boolean_T ChipSleep;                 /* 'ChipSleep' (':6077') */
  boolean_T ChipReset;                 /* 'ChipReset' (':6078') */
  boolean_T FeedBackBusStatus;         /* 'FeedBackBusStatus' (':6079') */
  int16_T TargetSpeedLimited;          /* 'TargetSpeedLimited' (':6080') */
  int8_T VoltageStatus;                /* 'VoltageStatus' (':6081') */
  int8_T AmbTStatus;                   /* 'AmbTStatus' (':6082') */
  uint8_T ShortCnt;                    /* 'ShortCnt' (':6083') */
  uint8_T OpenCnt;                     /* 'OpenCnt' (':6084') */
  uint8_T LockCnt;                     /* 'LockCnt' (':6085') */
  boolean_T PwmInError;                /* 'PwmInError' (':6086') */
  boolean_T PwmInShortGND;             /* 'PwmInShortGND' (':6087') */
  boolean_T PwmInShortBat;             /* 'PwmInShortBat' (':6088') */
  int8_T AmbTPreOverStatus;            /* 'AmbTPreOverStatus' (':6089') */
}
ExtY_T;

/* Real-time Model Data Structure */
struct tag_RTM_T
{
  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct
  {
    struct
    {
      uint8_T TID[2];
    }
    TaskCounters;
  }
  Timing;
};

/* Block signals (default storage) */
extern B_T B;

/* Block states (default storage) */
extern DW_T DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_T U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_T Y;

/* Constant parameters (default storage) */
extern const ConstP_T ConstP;

/* Model entry point functions */
extern void AppMain_initialize(void);
extern void AppMain_step(void);

/* Real-time Model object */
extern RT_MODEL_T *const M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block 'FaultDetectedLast' (':5353') : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block synthesized block : Eliminate redundant data type conversion
 * Block 'Manual Switch' (':5401') : Eliminated due to constant selection input
 * Block synthesized block : Eliminate redundant data type conversion
 * Block synthesized block : Eliminate redundant data type conversion
 * Block synthesized block : Eliminate redundant data type conversion
 * Block 'Manual Switch' (':5827') : Eliminated due to constant selection input
 * Block 'Manual Switch1' (':5828') : Eliminated due to constant selection input
 * Block 'Manual Switch2' (':5829') : Eliminated due to constant selection input
 * Block 'Manual Switch3' (':5830') : Eliminated due to constant selection input
 * Block 'Manual Switch' (':6024') : Eliminated due to constant selection input
 * Block synthesized block : Unused code path elimination
 * Block 'Constant' (':5340:3') : Unused code path elimination
 * Block 'Constant5' (':5816') : Unused code path elimination
 * Block 'Constant7' (':5818') : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is 'block_name' ('SID'), where block_name is the name of the block
 * and SID is the Simulink identifier of the block without the model name.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system(':3')    - opens block with Simulink identifier 'model:3'
 */

/*-
 * Requirements for model: AppMain
 */
#endif                                 /* RTW_HEADER_AppMain_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
