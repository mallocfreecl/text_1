/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:McLib.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
*
* (Requirements, pseudo code and etc.)
*********************************************************************
* Limitations:
*
* (limitations)
*********************************************************************
*********************************************************************
* Revision History：
*
* Version       Date         	  Author       Descriptions
* ----------    --------------  ------------   ------------
* 1.0           2022-03-30      yhd            Original
* 1.01          2022-05-23      yhd            1.McLibVer=4
*                                              2.修改高速自转检测
*                                              3.修改T1、T2计算值
*
********************************************************************
*END_FILE_HDR*/
#ifndef MC_LIB_H
#define MC_LIB_H

/* Includes*/
/******************************************************************/
//请勿修改下列头文件中已有定义
#include "DrvAdc.h"
#include "Mc.h"
#include "McCfg.h"
#include "McPhaseOffset.h"
#include "SdkTypes.h"

#include "tle_device.h"

/* Defines*/
/******************************************************************/
typedef struct
{
  // CC6x计数上升、下降比较值
  u16 comp60up;
  u16 comp61up;
  u16 comp62up;
  u16 comp60down;
  u16 comp61down;
  u16 comp62down;

  // SVM波形中的中T1、T2总时间的1/4，单位：Timer12计数值
  u16 T1;
  u16 T2;

  //电流采样时刻，单位：Timer21计数值
  u16 IShuntTrig1;
  u16 IShuntTrig2;
} PwmOutVar_t;

typedef struct
{
  // Buf[0]:缓冲数据，Buf[1]:实际使用数据
  PwmOutVar_t Buf[2u];
} PwmOutVarBuf_t;

typedef struct
{

  u8 Valid;        // 0：自转检查结果无效，1：自转检测结果有效
  s16 Dir;         //-1：反转，1:正转
  s16 Speed;       //带方向的自转转速，单位：rpm
  u16 Angle;       //加偏移后的自转角度
  u16 AngleOffset; //自转角度偏移

  //三相电压峰峰值，单位：AD
  s16 Upp;
  s16 Vpp;
  s16 Wpp;

  s16 BEMFMax; //线电压峰值,单位：AD
  s16 Ke;      // BEMFMax * MC_CFG_BEMF_FACTOR_SCALE / ABS(Speed)

  u16 TimeOutCnt;    //自转检测超时计数，单位：计数值
  u16 TimeOutCntMax; //自转检测超时阈值

  u16 ElecPeriodCnt;    //电周期计数值，单位：计数值
  u16 ElecPeriodCntMax; //电周期计数值最大值

  s32 ElecPeriodToSpeedCoef; // Speed = ElecPeriodToSpeedCoef / ElecPeriodCnt * Dir
} WmAPI_t;

/* Variables */
/******************************************************************/
extern PwmOutVarBuf_t PwmOutBuf;

/* Pwm波形上升、下降沿死区电压采样AD值 */
extern vu16 PhaseVoltageRisingEdge;
extern vu16 PhaseVoltageFallingEdge;

//电流采样电阻采样AD值，具体含义查看英飞凌参考文档：
// Infineon-TLE987x-Sensorless-Field-Oriented-Control-ApplicationNotes-v01_00-EN.pdf
extern s16 AdcResult0;
extern s16 AdcResult1;

//电机库版本号
extern const u16 McLibVer;

extern WmAPI_t WmAPI;


extern vu32 PhaseVoltageSum ;
extern vu16 PhaseVoltageCnt ;
extern vu16 DcVoltage ;

extern vu32 *pPhaseVoltageResultRegister;
/* Function declaration */
/******************************************************************/
/* 在Mc.h中已经申明 */
// extern void Mc_PwmStartHandler(void);
// extern void Mc_PwmPeriodHandler(void);

extern void Mc_exeSvm(McSvm_t *pSvm);
extern void Mc_T2C(u16 T1, u16 T2, u16 Ch_Squence);
extern void Mc_setPhaseVoltSample(void);
extern void Mc_exePhaseCtrl(u16 Sector);

extern void McWm_init(void);
extern void McWm_step(u16 VoltU, u16 VoltV, u16 VoltW);
extern u8 McWm_getSampleErrorFlag(void);

#endif
