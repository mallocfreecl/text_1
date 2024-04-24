/*
 * File: AppMain.h
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

#ifndef RTW_HEADER_AppMain_h_
#define RTW_HEADER_AppMain_h_
#ifndef AppMain_COMMON_INCLUDES_
#define AppMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AppMain_COMMON_INCLUDES_ */

#include "AppMain_types.h"
#include "PwmInTargetSpeedEval.h"
#include "FeedBack.h"
#include "rt_sys_ECUFaultDiag_0.h"
#include "MotorRestartAndFaultCnt.h"
#include "ECUFaultDiag.h"
#include <string.h>

/* Block signals (default storage) */
typedef struct
{
  uint16_T DutyLast;                   /* 'DutyLast' (':1800') */
  uint16_T Duty;                       /* 'Merge2' (':1760') */
  uint16_T Switch;                     /* 'Switch' (':3099') */
  uint16_T Freq;
  /* 'TmpSignal ConversionAtBusConversion_InsertedFor_PwmInStatus_at_inport_0_BusSelector1Outport3' (':0') */
  uint16_T HighDuty;
  /* 'TmpSignal ConversionAtBusConversion_InsertedFor_PwmInStatus_at_inport_0_BusSelector1Outport4' (':0') */
  uint16_T FalseKeepTime;              /* 'FeedBackTime' (':3199') */
  uint16_T TrueKeepTime;               /* 'FeedBackTime' (':3199') */
  uint16_T FaultRecoverCntOut;         /* 'FeedBackTime' (':3199') */
  int16_T PwmInTargetSpeed;            /* 'Merge1' (':1668') */
  int16_T Out;                         /* 'DelayOut' (':1398') */
  int8_T Status;                       /* 'AmbTPreOverDiag' (':3165') */
  int8_T Status_ila3;                  /* 'VoltageDiag' (':1620') */
  int8_T Status_mpxg;                  /* 'AmbTOverDiag' (':3164') */
  boolean_T Compare;                   /* 'Compare' (':2309:2') */
  boolean_T PwmInError;                /* 'Merge2' (':1669') */
  boolean_T Merge1;                    /* 'Merge1' (':1759') */
  boolean_T Err;                       /* 'Merge' (':1748') */
  boolean_T PwmInErr;                  /* 'Data Type Conversion1' (':1791') */
  boolean_T DutyValid;                 /* 'Logical Operator' (':2298') */
  boolean_T LogicalOperator;           /* 'Logical Operator' (':3238') */
  boolean_T State;                     /* 'StateKeep' (':2375') */
  boolean_T FeedBackFlagLast;          /* 'FeedBackTime' (':3199') */
  boolean_T FeedBackClearEn;           /* 'FeedBackTime' (':3199') */
  boolean_T ECUFault;                  /* 'Logical Operator' (':796') */
  B_DutyFilter_T DutyFilter_h2ru;      /* 'DutyFilter' (':1703') */
  B_FeedBackDelay_T FeedBackDelay5;    /* 'PowerUpBlank' (':2620') */
  B_FeedBackDelay_T FeedBackDelay4;    /* 'PowerUpBlank' (':2620') */
  B_FeedBackDelay_T PowerUpBlank;      /* 'PowerUpBlank' (':2620') */
}
B_T;

/* Block states (default storage) for system 'AppMain' */
typedef struct
{
  int16_T FltVoltageLast_PreviousInput;/* 'FltVoltageLast' (':2261') */
  int16_T LastSpeed_PreviousInput;     /* 'LastSpeed' (':1772') */
  uint16_T PowerOnBlankTimeCnt_PreviousInp;/* 'PowerOnBlankTimeCnt' (':967') */
  uint16_T DutyLast_PreviousInput;     /* 'DutyLast' (':1800') */
  uint16_T LastDelay_PreviousInput;    /* 'LastDelay' (':1773') */
  uint16_T LastState_PreviousInput;    /* 'LastState' (':1608') */
  uint16_T Memory_PreviousInput;       /* 'Memory' (':2603') */
  uint16_T FalseTime;                  /* 'StateKeep' (':2375') */
  uint16_T TrueTime;                   /* 'StateKeep' (':2375') */
  uint16_T local_hightime;             /* 'FeedBackTime' (':3199') */
  uint16_T local_lowtime;              /* 'FeedBackTime' (':3199') */
  uint16_T FaultRecoverCnt;            /* 'FeedBackTime' (':3199') */
  uint16_T FeedBackNumberLast;         /* 'FeedBackTime' (':3199') */
  boolean_T DelayInput1_DSTATE;        /* 'Delay Input1' (':2453:2') */
  boolean_T DelayInput1_DSTATE_ihcf;   /* 'Delay Input1' (':3097:2') */
  boolean_T DelayInput1_DSTATE_mx1q;   /* 'Delay Input1' (':1686:2') */
  boolean_T DelayInput1_DSTATE_h0ze;   /* 'Delay Input1' (':1687:2') */
  boolean_T Delay_DSTATE[2];           /* 'Delay' (':3237') */
  int8_T If_ActiveSubsystem;           /* 'If' (':1650') */
  int8_T If_ActiveSubsystem_aekd;      /* 'If' (':1676') */
  int8_T If1_ActiveSubsystem;          /* 'If1' (':1743') */
  uint8_T is_active_c3_AppMain;        /* 'StateKeep' (':2375') */
  uint8_T is_c3_AppMain;               /* 'StateKeep' (':2375') */
  uint8_T is_active_c4_AppMain;        /* 'FeedBackTime' (':3199') */
  uint8_T is_TimeJudge;                /* 'FeedBackTime' (':3199') */
  DW_AmbTOverDiag_T sf_AmbTPreOverDiag;/* 'AmbTOverDiag' (':3164') */
  DW_SpeedDelayOut_T SpeedDelayOut_awf2;/* 'SpeedDelayOut' (':1394') */
  DW_FreqValidJudge_T FreqValidJudge_llbg;/* 'FreqValidJudge' (':1734') */
  DW_DutyFilter_T DutyFilter_h2ru;     /* 'DutyFilter' (':1703') */
  DW_MotorRestartAndFaultCnt_T MotorRestartAndFaultCnt_bnwn;/* 'MotorRestartAndFaultCnt' (':815') */
  DW_FeedBackDelay_T FeedBackDelay5;   /* 'PowerUpBlank' (':2620') */
  DW_FeedBackDelay_T FeedBackDelay4;   /* 'PowerUpBlank' (':2620') */
  DW_FeedBackDelay_T PowerUpBlank;     /* 'PowerUpBlank' (':2620') */
  DW_ECUFaultDiag_T ECUFaultDiag_ima5; /* 'ECUFaultDiag' (':769') */
}
DW_T;

/* Constant parameters (default storage) */
typedef struct
{
  /* Expression: int16([256 256 256])
   * Referenced by: '1-D Lookup Table' (':2249')
   */
  int16_T uDLookupTable_tableData[3];

  /* Expression: int16([170 240 320])
   * Referenced by: '1-D Lookup Table' (':2249')
   */
  int16_T uDLookupTable_bp01Data[3];

  /* Expression: [CtrlConst.FeedBack.LowTime.PwmInError,CtrlConst.FeedBack.LowTime.Voltage,CtrlConst.FeedBack.LowTime.AmbT,CtrlConst.FeedBack.LowTime.MotorShort,CtrlConst.FeedBack.LowTime.MotorOpenLoad,CtrlConst.FeedBack.LowTime.MotorLock,CtrlConst.FeedBack.LowTime.MotorDryRunning,CtrlConst.FeedBack.LowTime.Normal]

   * Referenced by: 'BusLowTimeTable' (':2376')
   */
  uint16_T BusLowTimeTable_table[8];

  /* Expression: [CtrlConst.FeedBack.HighTime.PwmInError,CtrlConst.FeedBack.HighTime.Voltage,CtrlConst.FeedBack.HighTime.AmbT,CtrlConst.FeedBack.HighTime.MotorShort,CtrlConst.FeedBack.HighTime.MotorOpenLoad,CtrlConst.FeedBack.HighTime.MotorLock,CtrlConst.FeedBack.HighTime.MotorDryRunning,CtrlConst.FeedBack.HighTime.Normal]

   * Referenced by: 'BusHighTimeTable' (':2377')
   */
  uint16_T BusHighTimeTable_table[8];
}
ConstP_T;

/* External inputs (root inport signals with default storage) */
typedef struct
{
  int16_T SupVoltage;                  /* 'SupVoltage' (':766') */
  int16_T AmbTemperature;              /* 'AmbTemperature' (':767') */
  boolean_T ChipFault;                 /* 'ChipFault' (':768') */
  boolean_T PwmInPinSts;               /* 'PwmInPinSts' (':1038') */
  boolean_T SleepValid;                /* 'SleepValid' (':1566') */
  uint16_T PwmInFreq;                  /* 'PwmInFreq' (':1627') */
  uint16_T PwmInHighDuty;              /* 'PwmInHighDuty' (':1628') */
  uint16_T PwmInIdleTime;              /* 'PwmInIdleTime' (':1629') */
  boolean_T IsMotorShort;              /* 'IsMotorShort' (':1826') */
  boolean_T IsMotorLock;               /* 'IsMotorLock' (':1827') */
  boolean_T IsMotorOpenLoad;           /* 'IsMotorOpenLoad' (':1828') */
  boolean_T RestartComplete;           /* 'RestartComplete' (':1882') */
  int16_T FilteredVoltage;             /* 'FilteredVoltage' (':2253') */
  boolean_T IsMotorDryRunning;         /* 'IsMotorDryRunning' (':2342') */
  boolean_T TestModeSpeedCtrlEnable;   /* 'TestModeSpeedCtrlEnable' (':2915') */
  int16_T TestModeSpeed;               /* 'TestModeSpeed' (':2916') */
  uint16_T FeedBackCnt;                /* 'FeedBackCnt' (':3211') */
  uint16_T FeedBackClearComplete;      /* 'FeedBackClearComplete' (':3230') */
}
ExtU_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct
{
  boolean_T MotorRestartEnable;        /* 'MotorRestartEnable' (':1616') */
  boolean_T ChipSleep;                 /* 'ChipSleep' (':1625') */
  boolean_T ChipReset;                 /* 'ChipReset' (':1626') */
  boolean_T FeedBackBusStatus;         /* 'FeedBackBusStatus' (':2164') */
  int16_T TargetSpeedLimited;          /* 'TargetSpeedLimited' (':2216') */
  int8_T VoltageStatus;                /* 'VoltageStatus' (':2265') */
  int8_T AmbTStatus;                   /* 'AmbTStatus' (':2266') */
  uint8_T ShortCnt;                    /* 'ShortCnt' (':2584') */
  uint8_T OpenCnt;                     /* 'OpenCnt' (':2585') */
  uint8_T LockCnt;                     /* 'LockCnt' (':2586') */
  boolean_T PwmInError;                /* 'PwmInError' (':3085') */
  boolean_T PwmInShortGND;             /* 'PwmInShortGND' (':3087') */
  boolean_T PwmInShortBat;             /* 'PwmInShortBat' (':3089') */
  uint8_T DryRunningCnt;               /* 'DryRunningCnt' (':3162') */
  boolean_T FeedBackFlag;              /* 'FeedBackFlag' (':3186') */
  boolean_T FeedBackFlagLast;          /* 'FeedBackFlagLast' (':3214') */
  uint16_T FaultRecoverCntOut;         /* 'FaultRecoverCntOut' (':3218') */
  boolean_T FeedBackClearEn;           /* 'FeedBackClearEn' (':3227') */
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
 * Block 'FaultDetectedLast' (':2015') : Unused code path elimination
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
 * Block 'Manual Switch' (':3102') : Eliminated due to constant selection input
 * Block 'Manual Switch' (':3100') : Eliminated due to constant selection input
 * Block synthesized block : Eliminate redundant data type conversion
 * Block synthesized block : Eliminate redundant data type conversion
 * Block 'Constant' (':3103') : Unused code path elimination
 * Block synthesized block : Unused code path elimination
 * Block 'Constant' (':2950:3') : Unused code path elimination
 * Block 'Constant' (':1570') : Unused code path elimination
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
