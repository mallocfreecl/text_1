/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:Sdk.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.该文件提供操作硬件所需的接口，需要进行硬件相关操作时包含该文件
* 2.该文件的包含文件除了类型定义文件外，还包含对硬件操作进行抽象封装后文件，
* 但是这些文件应该只实现硬件功能，不包含应用逻辑
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
* 1.0           2021-12-09      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef SDK_H
#define SDK_H

/* Includes*/
/******************************************************************/
#include "DrvAdc.h"
#include "DrvPwmIn.h"
#include "SdkTypes.h"

/* Defines*/
/******************************************************************/
/* SDK_EXE_TIME_MEASURE_EN不为0时使用硬件定时器测量代码执行时间 */
#define SDK_EXE_TIME_MEASURE_EN (1u)

#if SDK_EXE_TIME_MEASURE_EN
/*
1.使用定时器执行时间测量时需要快速执行代码，所以将该部分代码定义写为内联函数。
2.该部分代码需要直接操作对应硬件，减少执行时间，所以不调用Sdk提供的方法
 */
#include "tle_device.h"

typedef struct
{
  u32 StartCnt;
  u32 EndCnt;
  u32 DiffCnt[3];
  u32 MaxDiffCnt[3];
  float Time_us[3];
} IsrTime_t;
#endif

typedef enum
{
  eAdcBat = 0x21u,
  eAdcVSD = 0x22u,
  eAdcVCP = 0x23u,
  eAdcMON = 0x24u,
  eAdcVDDP = 0x25u,
  eAdcVAREF = 0x26u,
  eAdcVBG = 0x27u,
  eAdcVDDC = 0x28u,
  eAdcChipT = 0x29u,
  eAdcAmbT = 0x14u,
  eAdcVoltDc = 0x30u,
  eAdcVoltU = 0x31u,
  eAdcVoltV = 0x32u,
  eAdcVoltW = 0x33u,
  eAdcIShunt = 0x34u,
} AdcCh_t;

/* polyspace-begin MISRA-C3:6.1 */
typedef union {
  u8 reg;

  struct
  {
    u8 ShortBAT : 1;
    u8 ShortGND : 1;
    u8 OpenLoadU : 1;
    u8 OpenLoadV : 1;
    u8 OpenLoadW : 1;
  } bit;
} BdrvSts_t;

typedef union {
  u32 reg;

  struct
  {
    u32 SysOT : 1;
    u32 PMU_5V_OV : 1;
    u32 PMU_1V5_OV : 1; // 2
    u32 SuppShort : 1;
    u32 SuppTimeOut : 1;

    u32 VDDEXT_OverLoad : 1; // 5
    u32 VDDEXT_OverVoltage : 1;
    u32 VDDEXT_Short : 1;

    u32 Ref5VOverLoad : 1;
    u32 Ref5VOverVoltage : 1;
    u32 Ref5VUnderVoltage : 1; // 11

    u32 VBGError : 1; // 13

    u32 VSDHigh : 1;
    u32 VSDLow : 1; // 20
    u32 VCPHigh : 1;
    u32 VCPLow1 : 1;
    u32 VCPLow2 : 1; // 23

    u32 ECUFault : 1;

    u32 DFlash : 1;
    u32 Ram : 1; // 26
    u32 CFlash : 1;
  } bit;
} ECUFaults_t;
/* polyspace-end MISRA-C3:6.1 */

/* Variables */
/******************************************************************/
#if SDK_EXE_TIME_MEASURE_EN
extern IsrTime_t IsrTime;
#endif
extern ECUFaults_t EcuFaults;
extern u32 CFlashChkSumCalc;

/* Function declaration */
/******************************************************************/
/* 硬件初始化 */
extern void Sdk_initHw(void);

/* 看门狗喂狗 */
extern void Sdk_feedWDog(void);
/* 芯片休眠 */
extern void Sdk_enterSleep(void);
/* 芯片复位 */
extern void Sdk_rstChip(void);
/* 关中断 */
extern void Sdk_enterCritical(void);
/* 开中断 */
extern void Sdk_exitCritical(void);

/* Pwm输出引脚设为高电平 */
extern void Sdk_setPwmOutPin(void);
/* Pwm输出引脚设为低电平 */
extern void Sdk_clrPwmOutPin(void);
/* Pwm输出引脚反转电平 */
extern void Sdk_togglePwmOutPin(void);
/* Pwm输出初始化 */
extern void Sdk_initPwmOut(void);
/* Pwm输出高电平 */
extern void Sdk_setPwmOut(void);
/* Pwm输出低电平 */
extern void Sdk_clrPwmOut(void);

/* 通过串口读取一个Byte */
extern u8 Sdk_readUartU8(void);
/* 向串口写入一个Byte，返回值：0：写入成功，1：写入失败 */
extern u8 Sdk_writeUartU8(u8 Data);

/* 获取Adc中Ch通道的采样值，返回值为ADC值，pData：用于保存结果的指针,如果ADC数据无效则不更新pData */
extern void Sdk_getAdcRawData(AdcCh_t Ch, u16 *pData);

extern u8 Sdk_getIGSts(void);

extern u8 Sdk_isPwmInCap(void);
/* 获取Pwm输入捕获定时器的捕获值 */
extern u16 Sdk_getPwmInCapVal(void);
/* 设置捕获定时器上升沿捕获 */
extern void Sdk_setPwmInCapRisingEdge(void);
/* 设置捕获定时器下降沿捕获 */
extern void Sdk_setPwmInCaptureFallingEdge(void);
/* 获取捕获定时器的捕获边沿，0：下降沿，1：上升沿 */
extern u8 Sdk_getPwmInCapEdge(void);
/* 捕获定时器是否运行，0：停止，1：上升 */
extern u8 Sdk_isPwmInIsCapTmrRunning(void);
/* 复位捕获定时器 */
extern void Sdk_rstPwmInCapTmr(void);

/* 获取Pwm输入引脚的状态，0：低电平，1：高电平 */
extern u8 Sdk_getPwmInBusSts(void);

/* 打开桥驱 */
extern void Sdk_openBdrv(void);
/* 关闭桥驱并使相电压为接地 */
extern void Sdk_closeBdrv(void);
/* 关闭桥驱并使相电压为悬空 */
extern void Sdk_floatBdrv(void);
/* 清除桥驱的错误 */
extern void Sdk_clrBdrvError(void);
/* 设置三相桥的输出占空比，输入单位：0.01% */
extern void Sdk_setBdrvDuty(u16 DutyU, u16 DutyV, u16 DutyW);
/* 开始BDRV测试 */
extern void Sdk_startBdrvTest(void);
/* U:Pwm,V:High,W:Float */
extern void Sdk_openBdrvUV(void);
/* U:Float,V:Pwm,W:High */
extern void Sdk_openBdrvVW(void);
/* U:High,V:Float,W:Pwm */
extern void Sdk_openBdrvWU(void);
/* 结束BDRV测试 */
extern void SDK_stopBdrvTest(void);
/* U相输出低电平 */
extern void Sdk_setULow(void);
/* U相输出低电平 */
extern void Sdk_setUHigh(void);
/* 获取Bdrv打开状态下的短路状态，并将状态更新到DrvSts中。返回值0：诊断未完成，1：诊断完成 */
extern u8 Sdk_diagBdrvOnShort(BdrvSts_t *DrvSts);

/* 电机运行前设置硬件 */
extern void Sdk_startMotorHw(void);
/* 电机停止前设置硬件 */
extern void Sdk_stopMotorHw(void);

/* 获取数据flash的起始地址 */
extern Addr_t Sdk_getDFlashStartAddr(void);
/*获取备份数据地址 22.04.21 by jxj*/
extern Addr_t Sdk_getDFlashBackUpAddr(void);
/* 向数据flash写入数据，DataLen单位：Byte */
extern void Sdk_writeDFlash(Addr_t Addr, u8 *pData, u16 DataLen);
/* 从数据flash读出数据，DataLen单位：Byte */
extern void Sdk_readDFlash(Addr_t Addr, u8 *pData, u16 DataLen);
/* 将数据flash中从Addr开始，长度为DataLen的数据清除，DataLen单位：Bytes */
extern void Sdk_eraseDFlash(Addr_t Addr, u16 DataLen);
/* 恢复DFalsh故障，Addr:故障区域起始地址 ；DataLen故障区域长度，单位：Bytes*/
extern void Sdk_clrDFlashError(Addr_t Addr, u16 DataLen);

/* 检测芯片是否存在故障 */
extern void Sdk_diagEcuFault(void);

#if SDK_EXE_TIME_MEASURE_EN
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_startExeTimeMeasure
* Description:
* 1.在被测代码的其实位置调用该函数
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
* 1.该函数不可嵌套
*
********************************************************************
END_FUNCTION_HDR*/
SDK_STATIC_INLINE void Sdk_startExeTimeMeasure(void)
{
  IsrTime.StartCnt = CPU->SYSTICK_CUR.reg;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_calcExeTime
* Description:
* 1.计算代码的执行时间，在被测代码结束位置调用
*
* Inputs:
* idx:设置将执行时间保存到数组中的索引
*
* Outputs:
* IsrTime.Time_us[idx]：代码执行时间，单位：us
*
* Limitations:
* 1.该函数不可嵌套
*
********************************************************************
END_FUNCTION_HDR*/
SDK_STATIC_INLINE void Sdk_calcExeTime(u8 idx)
{
  IsrTime.EndCnt = CPU->SYSTICK_CUR.reg;
  IsrTime.DiffCnt[idx] =
    ((IsrTime.EndCnt < IsrTime.StartCnt) ? 0u : CPU->SYSTICK_RL.reg) + IsrTime.StartCnt - IsrTime.EndCnt;
  if (IsrTime.DiffCnt[idx] > IsrTime.MaxDiffCnt[idx])
  {
    IsrTime.MaxDiffCnt[idx] = IsrTime.DiffCnt[idx];
    // IsrTime.Time_us[idx] = (float)IsrTime.MaxDiffCnt[idx] / ((SCU_FSYS) / 1000000u);
  }
}
#endif

#endif // !SDK_H
